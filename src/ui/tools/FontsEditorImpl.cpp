#include "FontsEditorImpl.h"
#include "metro/MetroFonts.h"
#include "metro/MetroLocalization.h"
#include "metro/MetroTexture.h"
#include "metro/MetroTexturesDatabase.h"
#include "metro/MetroBinArchive.h"
#include "metro/MetroConfigDatabase.h"
#include "metro/MetroFileSystem.h"
#include "metro/reflection/MetroReflection.h"

#include "ui/UIHelpers.h"

#include <fstream>

namespace MetroEX {

FontsEditorImpl::FontsEditorImpl() {
    mCharsList = gcnew System::Collections::Generic::List<wchar_t>();
    mFontBitmap = nullptr;
    mSelectedCharBrush = gcnew System::Drawing::SolidBrush(System::Drawing::Color::FromArgb(105, 173, 216, 230));
    mSelectedLanguage = -1;
    mSelectedFont = -1;
    mSelectedChar = -1;
    mPreviewWnd = nullptr;
}
FontsEditorImpl::~FontsEditorImpl() {
}


void FontsEditorImpl::OnFormLoaded() {
    this->cmbLanguages->SelectedIndex = -1;
    this->cmbLanguages->Items->Clear();
    this->cmbLanguages->Sorted = false; // just for sanity

    this->lstFonts->SelectedIndex = -1;
    this->lstFonts->Items->Clear();
    this->lstFonts->Sorted = false; // just for sanity

    mSelectedLanguage = -1;
    mSelectedFont = -1;
    mSelectedChar = -1;

    this->imagePanel->OnPostDraw += gcnew MetroEXControls::ImagePanel::OnPostDrawDelegate(this, &FontsEditorImpl::OnFontTexturePostDraw);
    this->imagePanel->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &FontsEditorImpl::OnFontTextureClick);
    this->imagePanel->TransparencyEnabled = false;

    for (size_t i = scast<size_t>(MetroLanguage::First); i <= scast<size_t>(MetroLanguage::Last); ++i) {
        System::String^ lngName = ToNetString(MetroLanguagesFullName[i]);
        this->cmbLanguages->Items->Add(lngName);
    }

    // select English by default
    this->cmbLanguages->SelectedIndex = scast<size_t>(MetroLanguage::US);
}

void FontsEditorImpl::OnLanguageSelected(int idx) {
    if (idx >= scast<int>(MetroLanguage::First) && idx <= scast<int>(MetroLanguage::Last)) {
        this->lstFonts->SelectedIndex = -1;
        this->lstFonts->Items->Clear();
        this->lstFonts->Sorted = false; // just for sanity

        mSelectedLanguage = idx;
        mSelectedFont = -1;
        mSelectedChar = -1;

        MetroFontsDatabase& mfdb = MetroFontsDatabase::Get(scast<MetroLanguage>(idx));
        if (mfdb.GetFontsCount()) {
            for (size_t i = 0; i < mfdb.GetFontsCount(); ++i) {
                const MetroFontDescriptor& mfdesc = mfdb.GetFontDescriptor(i);
                this->lstFonts->Items->Add(ToNetString(mfdesc.GetName()));
            }

            mCharsList->Clear();
            CharString locPath = MetroFileSystem::Paths::LocalizationsFolder + "stable_" + MetroLanguagesStr[idx] + ".lng";
            MetroLocalization loc;
            loc.LoadFromPath(locPath);

            for (size_t i = 0; i < loc.GetCharsCount(); ++i) {
                const wchar_t ch = loc.GetChar(i);
                mCharsList->Add(ch);
            }

            // select the first one
            this->lstFonts->SelectedIndex = 0;
        }
    }
}

void FontsEditorImpl::OnFontSelected(int idx) {
    const MetroLanguage lng = scast<MetroLanguage>(mSelectedLanguage);
    MetroFontsDatabase& mfdb = MetroFontsDatabase::Get(lng);
    MetroFontDescriptor& mfdesc = mfdb.GetFontDescriptor(scast<size_t>(idx));

    this->lblLanguage->Text = ToNetString(MetroLanguagesStr[scast<size_t>(lng)]);
    this->lblName->Text = ToNetString(mfdesc.GetName());
    this->lblTexture->Text = ToNetString(mfdesc.GetTextureName());
    this->lblInvScale->Text = ToNetString(std::to_string(mfdesc.GetInvScale().x) + ", " + std::to_string(mfdesc.GetInvScale().y));
    this->lblLineHeight->Text = ToNetString(std::to_string(mfdesc.GetLineHeight()));
    this->lblCharsCount->Text = ToNetString(std::to_string(mfdesc.GetCharsCount()));

    mSelectedFont = idx;
    mSelectedChar = -1;

    mFontBitmap = nullptr;

    const CharString& realTextureName = MetroTexturesDatabase::Get().GetSourceName(mfdesc.GetTextureName());
    const MetroTextureInfo* mti = MetroTexturesDatabase::Get().GetInfoByName(realTextureName);
    if (mti) {
        CharString texturePath = MetroFileSystem::Paths::TexturesFolder + realTextureName + "." + (mti->streamable ? std::to_string(mti->width) : "dds");
        MetroTexture texture;
        if (texture.LoadFromPath(texturePath)) {
            BytesArray pixels;
            texture.GetBGRA(pixels);

            const int w = scast<int>(texture.GetWidth());
            const int h = scast<int>(texture.GetHeight());

            mFontBitmap = gcnew System::Drawing::Bitmap(w, h, System::Drawing::Imaging::PixelFormat::Format32bppArgb);

            System::Drawing::Rectangle rc(0, 0, w, h);
            System::Drawing::Imaging::BitmapData^ bmpData = mFontBitmap->LockBits(rc, System::Drawing::Imaging::ImageLockMode::WriteOnly, mFontBitmap->PixelFormat);
            memcpy(bmpData->Scan0.ToPointer(), pixels.data(), pixels.size());
            mFontBitmap->UnlockBits(bmpData);

            this->imagePanel->Image = mFontBitmap;
        }
    }

    this->UpdatePreviewWindow(true);

    this->OnCharInListSelected(mSelectedChar);
}

void FontsEditorImpl::OnCharValueChanged(System::Object^ sender) {
    if (mSelectedLanguage >= 0 && mSelectedFont >= 0 && mSelectedChar >= 0) {
        const MetroLanguage lng = scast<MetroLanguage>(mSelectedLanguage);
        MetroFontsDatabase& mfdb = MetroFontsDatabase::Get(lng);
        MetroFontDescriptor& mfdesc = mfdb.GetFontDescriptor(scast<size_t>(mSelectedFont));
        MetroFontDescriptor::CharInfo& ci = mfdesc.GetCharinfo(scast<size_t>(mSelectedChar));

        if (sender == this->numPosX) {
            ci.x = scast<uint16_t>(System::Decimal::ToInt16(this->numPosX->Value));
        } else if (sender == this->numPosY) {
            ci.y = scast<uint16_t>(System::Decimal::ToInt16(this->numPosY->Value));
        } else if (sender == this->numWidth) {
            ci.width = scast<uint16_t>(System::Decimal::ToInt16(this->numWidth->Value));
        } else if (sender == this->numHeight) {
            ci.height = scast<uint16_t>(System::Decimal::ToInt16(this->numHeight->Value));
        } else if (sender == this->numOffX) {
            ci.xoffset = scast<uint16_t>(System::Decimal::ToInt16(this->numOffX->Value));
        } else if (sender == this->numOffY) {
            ci.yoffset = scast<uint16_t>(System::Decimal::ToInt16(this->numOffY->Value));
        } else if (sender == this->numXAdvance) {
            ci.xadvance = scast<uint16_t>(System::Decimal::ToInt16(this->numXAdvance->Value));
        }

        this->imagePanel->Invalidate();
        this->UpdatePreviewWindow(true);
    }
}

void FontsEditorImpl::OnLanguageExport() {
    if (mSelectedLanguage >= 0) {
        const MetroLanguage lng = scast<MetroLanguage>(mSelectedLanguage);
        MetroFontsDatabase& mfdb = MetroFontsDatabase::Get(lng);
        if (mfdb.GetFontsCount() > 0) {
            System::Windows::Forms::SaveFileDialog sfd;
            sfd.Title = L"Save Metro font language database...";
            sfd.Filter = L"Bin file (*.bin)|*.bin";
            sfd.RestoreDirectory = true;
            sfd.OverwritePrompt = true;
            if (sfd.ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
                fs::path outPath = StringToPath(sfd.FileName);

                MemWriteStream stream;
                mfdb.SaveToData(stream);

                std::ofstream binFile(outPath, std::ofstream::binary);
                binFile.write(rcast<const char*>(stream.Data()), stream.GetWrittenBytesCount());
                binFile.flush();
                binFile.close();
            }
        }
    }
}

void FontsEditorImpl::OnLanguageImport() {
    if (mSelectedLanguage >= 0) {
        const MetroLanguage lng = scast<MetroLanguage>(mSelectedLanguage);
        MetroFontsDatabase& mfdb = MetroFontsDatabase::Get(lng);

        System::Windows::Forms::OpenFileDialog ofd;
        ofd.Title = L"Load Metro font language database...";
        ofd.Filter = L"Bin file (*.bin)|*.bin";
        ofd.RestoreDirectory = true;
        ofd.CheckFileExists = true;
        if (ofd.ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
            fs::path inPath = StringToPath(ofd.FileName);

            BytesArray fileData;
            std::ifstream binFile(inPath, std::ifstream::binary);
            binFile.seekg(0, std::ios::end);
            fileData.resize(binFile.tellg());
            binFile.seekg(0, std::ios::beg);
            binFile.read(rcast<char*>(fileData.data()), fileData.size());
            binFile.close();

            MemStream stream(fileData.data(), fileData.size());
            mfdb.LoadFromData(stream);

            this->imagePanel->Invalidate();
        }
    }
}

void FontsEditorImpl::OnLanguageSave() {
    if (mSelectedLanguage >= 0) {
        const MetroLanguage lng = scast<MetroLanguage>(mSelectedLanguage);
        MetroFontsDatabase& mfdb = MetroFontsDatabase::Get(lng);
        if (mfdb.GetFontsCount() > 0) {
            bool result = false;

            CharString fontDBPath = MetroFontsDatabase::MakeFontDBPath(lng);
            const MetroConfigsDatabase::ConfigInfo* ci = MetroConfigsDatabase::Get().FindFile(fontDBPath);
            if (ci) {
                MemWriteStream outStream;
                mfdb.SaveToData(outStream);

                MemStream tempStream(outStream.Data(), outStream.GetWrittenBytesCount());
                result = MetroConfigsDatabase::Get().ReplaceFileByIdx(ci->idx, tempStream);
            }

            if (!result) {
                MetroEX::ShowErrorMessageBox(this, "Failed to save fonts database\nfor this language!");
            } else {
                MetroEX::ShowInfoMessageBox(this, "Fonts database was successfully saved!");
            }
        }
    }
}

void FontsEditorImpl::OnTestFont() {
    if (!mPreviewWnd && mSelectedLanguage >= 0 && mSelectedFont >= 0) {
        mPreviewWnd = gcnew MetroEXControls::FontPreviewWnd();
        mPreviewWnd->Closed += gcnew System::EventHandler(this, &FontsEditorImpl::OnPreviewWndClosed);
        mPreviewWnd->Icon = this->Icon;

        this->UpdatePreviewWindow(false);

        mPreviewWnd->Show();
    }
}


void FontsEditorImpl::OnFontTexturePostDraw(System::Drawing::Graphics^ g) {
    if (mSelectedLanguage >= 0 && mSelectedFont >= 0 && this->imagePanel->Image) {
        const MetroLanguage lng = scast<MetroLanguage>(mSelectedLanguage);
        MetroFontsDatabase& mfdb = MetroFontsDatabase::Get(lng);
        const MetroFontDescriptor& mfdesc = mfdb.GetFontDescriptor(scast<size_t>(mSelectedFont));

        const int offX = this->imagePanel->GetDrawOffsetX();
        const int offY = this->imagePanel->GetDrawOffsetY();

        const float scaleX = scast<float>(this->imagePanel->Image->Width) * mfdesc.GetInvScale().x;
        const float scaleY = scast<float>(this->imagePanel->Image->Height) * mfdesc.GetInvScale().y;

        g->CompositingMode = System::Drawing::Drawing2D::CompositingMode::SourceOver;

        for (size_t i = 0; i < mfdesc.GetCharsCount(); ++i) {
            const MetroFontDescriptor::CharInfo& ci = mfdesc.GetCharinfo(i);

            //#NOTE_SK: why in the actual fuck do we store them as u16 in the first place ???
            const int charX = scast<int16_t>(ci.x);
            const int charY = scast<int16_t>(ci.y);

            const int x = offX + scast<int>(charX * scaleX);
            const int y = offY + scast<int>(charY * scaleY);
            const int xx = x + scast<int>(ci.width * scaleX);
            const int yy = y + scast<int>(ci.height * scaleY);

            if (i == mSelectedChar) {
                g->FillRectangle(mSelectedCharBrush, x, y, scast<int>(ci.width * scaleX), scast<int>(ci.height * scaleY));
                g->DrawRectangle(System::Drawing::Pens::Blue, x, y, scast<int>(ci.width * scaleX), scast<int>(ci.height * scaleY));
            } else {
                g->DrawRectangle(System::Drawing::Pens::BlueViolet, x, y, scast<int>(ci.width * scaleX), scast<int>(ci.height * scaleY));
            }
        }
    }
}

void FontsEditorImpl::OnFontTextureClick(System::Object^, System::Windows::Forms::MouseEventArgs^ e) {
    const int offX = this->imagePanel->GetDrawOffsetX();
    const int offY = this->imagePanel->GetDrawOffsetY();

    if (mSelectedLanguage >= 0 && mSelectedFont >= 0) {
        const MetroLanguage lng = scast<MetroLanguage>(mSelectedLanguage);
        MetroFontsDatabase& mfdb = MetroFontsDatabase::Get(lng);
        const MetroFontDescriptor& mfdesc = mfdb.GetFontDescriptor(scast<size_t>(mSelectedFont));

        const int offX = this->imagePanel->GetDrawOffsetX();
        const int offY = this->imagePanel->GetDrawOffsetY();

        const float scaleX = scast<float>(this->imagePanel->Image->Width) * mfdesc.GetInvScale().x;
        const float scaleY = scast<float>(this->imagePanel->Image->Height) * mfdesc.GetInvScale().y;

        int selectedChar = mSelectedChar;
        for (size_t i = 0; i < mfdesc.GetCharsCount(); ++i) {
            const MetroFontDescriptor::CharInfo& ci = mfdesc.GetCharinfo(i);

            //#NOTE_SK: why in the actual fuck do we store them as u16 in the first place ???
            const int charX = scast<int16_t>(ci.x);
            const int charY = scast<int16_t>(ci.y);

            const int x = offX + scast<int>(charX * scaleX);
            const int y = offY + scast<int>(charY * scaleY);
            const int xx = x + scast<int>(ci.width * scaleX);
            const int yy = y + scast<int>(ci.height * scaleY);

            if (e->X >= x && e->X <= xx && e->Y >= y && e->Y <= yy) {
                selectedChar = scast<int>(i);
                break;
            }
        }

        if (mSelectedChar != selectedChar) {
            this->OnCharInListSelected(selectedChar);
        }
    }
}

void FontsEditorImpl::OnCharInListSelected(int idx) {
    mSelectedChar = idx;

    if (mSelectedChar >= 0) {
        const MetroLanguage lng = scast<MetroLanguage>(mSelectedLanguage);
        MetroFontsDatabase& mfdb = MetroFontsDatabase::Get(lng);
        const MetroFontDescriptor& mfdesc = mfdb.GetFontDescriptor(scast<size_t>(mSelectedFont));
        const MetroFontDescriptor::CharInfo& ci = mfdesc.GetCharinfo(scast<size_t>(mSelectedChar));

        this->numPosX->Value = scast<int16_t>(ci.x);
        this->numPosY->Value = scast<int16_t>(ci.y);
        this->numWidth->Value = scast<int16_t>(ci.width);
        this->numHeight->Value = scast<int16_t>(ci.height);
        this->numOffX->Value = scast<int16_t>(ci.xoffset);
        this->numOffY->Value = scast<int16_t>(ci.yoffset);
        this->numXAdvance->Value = scast<int16_t>(ci.xadvance);

        const wchar_t selectedChar = (mfdesc.GetCharsCount() == mCharsList->Count) ? mCharsList[idx] : scast<wchar_t>(idx);
        this->lblChar->Text = selectedChar.ToString();
    } else {
        this->numPosX->Value = 0;
        this->numPosY->Value = 0;
        this->numWidth->Value = 0;
        this->numHeight->Value = 0;
        this->numOffX->Value = 0;
        this->numOffY->Value = 0;
        this->numXAdvance->Value = 0;

        this->lblChar->Text = "";
    }

    this->imagePanel->Invalidate();
}

void FontsEditorImpl::OnPreviewWndClosed(System::Object^, System::EventArgs^) {
    MySafeDelete(mPreviewWnd);
}

void FontsEditorImpl::UpdatePreviewWindow(const bool redraw) {
    if (mPreviewWnd != nullptr) {
        mPreviewWnd->mImage = this->imagePanel->Image;
        mPreviewWnd->mCharsMap = mCharsList;

        const MetroLanguage lng = scast<MetroLanguage>(mSelectedLanguage);
        MetroFontsDatabase& mfdb = MetroFontsDatabase::Get(lng);
        MetroFontDescriptor& mfdesc = mfdb.GetFontDescriptor(scast<size_t>(mSelectedFont));

        mPreviewWnd->mLineHeight = scast<int>(mfdesc.GetLineHeight());

        const float scaleX = scast<float>(this->imagePanel->Image->Width) * mfdesc.GetInvScale().x;
        const float scaleY = scast<float>(this->imagePanel->Image->Height) * mfdesc.GetInvScale().y;
        mPreviewWnd->mScale.X = scaleX;
        mPreviewWnd->mScale.Y = scaleY;

        mPreviewWnd->mCharsDB = gcnew System::Collections::Generic::List<MetroEXControls::CharInfo>();
        for (size_t i = 0, end = mfdesc.GetCharsCount(); i < end; ++i) {
            const MetroFontDescriptor::CharInfo& ci = mfdesc.GetCharinfo(i);

            MetroEXControls::CharInfo testCI;
            testCI.x = scast<int16_t>(ci.x);
            testCI.y = scast<int16_t>(ci.y);
            testCI.width = scast<int16_t>(ci.width);
            testCI.height = scast<int16_t>(ci.height);
            testCI.xoffset = scast<int16_t>(ci.xoffset);
            testCI.yoffset = scast<int16_t>(ci.yoffset);
            testCI.xadvance = scast<int16_t>(ci.xadvance);

            mPreviewWnd->mCharsDB->Add(testCI);
        }

        if (redraw) {
            mPreviewWnd->Redraw();
        }
    }
}

} // namespace MetroEX
