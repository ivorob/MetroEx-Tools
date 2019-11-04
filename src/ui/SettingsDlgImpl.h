#pragma once

namespace MetroEX {

    public ref class SettingsDlgImpl : public MetroEXControls::SettingsDlg {
    public:
        SettingsDlgImpl();
        virtual ~SettingsDlgImpl();

    protected:
        virtual void OnFormLoaded() override;
        virtual void OnOKButtonClicked() override;
    };

} // namespace MetroEX
