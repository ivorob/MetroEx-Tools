#pragma once
#include "mycommon.h"

namespace MetroEX {

    public ref class ConvertTexturesDlgImpl : public MetroEXControls::ConvertTexturesDlg {
    public:
        ConvertTexturesDlgImpl();
        virtual ~ConvertTexturesDlgImpl();

    protected:
        virtual void                OnChooseFolderClicked() override;
        virtual void                OnConvertClicked() override;
        virtual void                OnStopClicked() override;

    private:
        // conversion
        bool                        ConvertOneFile(const fs::path& path);
        void                        ConvertFolder(const fs::path& path);
        void                        ConversionThreadFunc(System::Object^ pathStr);

        // logging
        void                        LogAction(System::Object^ str);

        // threading stuff
        void                        DisableUI();
        void                        EnableUI();
        void                        InitProgressBar(System::Object^ obj);
        void                        StepProgressBar();

    private:
        volatile bool               mConversionInProgress;
        volatile bool               mStopRequested;
        System::Threading::Thread^  mConversionThread;
        bool                        mIncludeSubfolders;
    };

} // namespace MetroEX
