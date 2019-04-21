#include "metro/MetroLocalization.h"

#include "LocalizationPanel.h"

#include "UIHelpers.h"

namespace MetroEX {

    void LocalizationPanel::SetLocalizationTable(const MetroLocalization* loc) {
        this->lstStringsTable->Items->Clear();

        const size_t numStrings = loc->GetNumStrings();

        this->lstStringsTable->BeginUpdate();
        for (size_t i = 0; i < numStrings; ++i) {
            this->lstStringsTable->Items->Add(ToNetString(loc->GetKey(i)))->SubItems->Add(ToNetString(loc->GetValue(i)));
        }
        this->lstStringsTable->EndUpdate();
    }

} // namespace MetroEX
