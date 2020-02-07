#pragma once
#include "metro/MetroTypes.h"

namespace MetroEX {

    public ref class FontsEditorImpl : public MetroEXControls::FontsEditor {
        ref struct FontBitmap {
            int width;
            int height;
        };

    public:
        FontsEditorImpl();
        ~FontsEditorImpl();

    protected:
        virtual void OnFormLoaded() override;
        virtual void OnLanguageSelected(int idx) override;
        virtual void OnFontSelected(int idx) override;
        virtual void OnCharValueChanged(System::Object^ sender) override;
        virtual void OnLanguageExport() override;
        virtual void OnLanguageImport() override;
        virtual void OnLanguageSave() override;
        virtual void OnTestFont() override;

        void OnFontTexturePostDraw(System::Drawing::Graphics^ g);
        void OnFontTextureClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
        void OnCharInListSelected(int idx);
        void OnPreviewWndClosed(System::Object^ sender, System::EventArgs^ e);

        void UpdatePreviewWindow(const bool redraw);

    private:
        System::Collections::Generic::List<wchar_t>^    mCharsList;
        System::Drawing::Bitmap^                        mFontBitmap;
        System::Drawing::Brush^                         mSelectedCharBrush;
        int                                             mSelectedLanguage;
        int                                             mSelectedFont;
        int                                             mSelectedChar;
        MetroEXControls::FontPreviewWnd^                mPreviewWnd;
    };

} // namespace MetroEX
