//
// Created by pierre on 7/20/26.
//

#ifndef X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_MODULESUMMARY_HPP
#define X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_MODULESUMMARY_HPP

#include <QGroupBox>
#include <QWidget>

#include "summaryitembase.hpp"
#include "stationbuilder/Complex.hpp"
#include "utils/modules.hpp"
#include "utils/MultiLayout.hpp"
#include "utils/SortedLayout.hpp"

namespace ui::section::summarysection::widgets {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class ModuleSummary;
    }

    QT_END_NAMESPACE

    struct ModuleSummaryItemData {
        const common::types::module::Module module;
        size_t                              amount;
    };

    class ModuleSummaryPriceItem final : public SummaryItemBase {
    public:
        ModuleSummaryPriceItem(ModuleSummaryItemData data);

        const ModuleSummaryItemData data;

        struct Comparator {
            static bool operator()(const ModuleSummaryPriceItem *a, const ModuleSummaryPriceItem *b);
        };
    };

    class ModuleSummaryNameItem final : public SummaryItemBase {
    public:
        ModuleSummaryNameItem(ModuleSummaryItemData data);

        const ModuleSummaryItemData data;

        struct Comparator {
            static bool operator()(const ModuleSummaryNameItem *a, const ModuleSummaryNameItem *b);
        };
    };

    class ModuleSummaryCountItem final : public SummaryItemBase {
    public:
        ModuleSummaryCountItem(ModuleSummaryItemData data);

        const ModuleSummaryItemData data;

        struct Comparator {
            static bool operator()(const ModuleSummaryCountItem *a, const ModuleSummaryCountItem *b);
        };
    };

    class ModuleSummary : public QGroupBox {
        Q_OBJECT

    public:
        using t_x4_complex = common::stationbuilder::t_x4_complex;

        explicit ModuleSummary(QWidget *parent = nullptr);

        ~ModuleSummary() override;

        void update(const common::stationbuilder::Complex &value, const common::data::Store &store);

    private:
        Ui::ModuleSummary *ui;

        using price_layout = utils::SortedLayout<ModuleSummaryPriceItem, ModuleSummaryPriceItem::Comparator>;
        using name_layout  = utils::SortedLayout<ModuleSummaryNameItem, ModuleSummaryNameItem::Comparator>;
        using count_layout = utils::SortedLayout<ModuleSummaryCountItem, ModuleSummaryCountItem::Comparator>;

        utils::MultiLayout<price_layout, name_layout, count_layout> layouts;
        QLayout *                                                   content_layout;
        int                                                         current_layout_index;

        void updateDisplay_(int idx);
    };
} // ui::section::summarysection::widgets

#endif //X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_MODULESUMMARY_HPP
