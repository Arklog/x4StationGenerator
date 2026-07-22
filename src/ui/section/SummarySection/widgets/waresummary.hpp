//
// Created by pierre on 7/21/26.
//

#ifndef X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_WARESUMMARY_HPP
#define X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_WARESUMMARY_HPP

#include <QGroupBox>
#include <QWidget>

#include "summaryitembase.hpp"
#include "common/types/Ware.hpp"
#include "stationbuilder/Complex.hpp"
#include "utils/MultiLayout.hpp"
#include "utils/SortedLayout.hpp"
#include "utils/WareTargetContainer.hpp"

namespace ui::section::summarysection::widgets {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class WareSummary;
    }

    QT_END_NAMESPACE

    struct WareSummaryItemData {
        common::types::Ware ware;
        int                 amount;
    };

    class WareSummaryNameItem : public SummaryItemBase {
    public:
        WareSummaryNameItem(WareSummaryItemData data_);

        WareSummaryItemData data;

        struct Comparator {
            static bool operator()(const WareSummaryNameItem *a, const WareSummaryNameItem *b);
        };
    };

    class WareSummaryPriceItem : public SummaryItemBase {
    public:
        WareSummaryPriceItem(WareSummaryItemData data_);

        WareSummaryItemData data;
        long long           avg_price;

        struct Comparator {
            static bool operator()(const WareSummaryPriceItem *a, const WareSummaryPriceItem *b);
        };
    };

    class WareSummaryAmountItem : public SummaryItemBase {
    public:
        WareSummaryAmountItem(WareSummaryItemData data_);

        WareSummaryItemData data;

        struct Comparator {
            static bool operator()(const WareSummaryAmountItem *a, const WareSummaryAmountItem *b);
        };
    };

    class WareSummary : public QGroupBox {
        Q_OBJECT

    public:
        explicit WareSummary(QWidget *parent = nullptr);

        ~WareSummary() override;

        void update(const common::stationbuilder::Complex &ware_targets, const common::data::Store &store);

    private:
        Ui::WareSummary *ui;

        using price_layout  = utils::SortedLayout<WareSummaryPriceItem, WareSummaryPriceItem::Comparator>;
        using amount_layout = utils::SortedLayout<WareSummaryAmountItem, WareSummaryAmountItem::Comparator>;
        using name_layout   = utils::SortedLayout<WareSummaryNameItem, WareSummaryNameItem::Comparator>;

        utils::MultiLayout<price_layout, amount_layout, name_layout> layouts;
        QLayout *                                                    layout;
        int                                                          current_index;

        void updateDisplay_();
    };
} // ui::section::summarysection::widgets

#endif //X4STATIONGENERATOR_SRC_UI_SECTION_SUMMARYSECTION_WIDGETS_WARESUMMARY_HPP
