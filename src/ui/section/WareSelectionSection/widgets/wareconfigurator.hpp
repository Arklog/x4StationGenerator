//
// Created by pierre on 7/17/25.
//

#ifndef WARECONFIGURATOR_HPP
#define WARECONFIGURATOR_HPP

#include <QFrame>
#include <QWidget>

#include "Data/RawData.hpp"

#include "StationBuilder/defines.hpp"

QT_BEGIN_NAMESPACE

namespace Ui {
class WareConfigurator;
}

QT_END_NAMESPACE

class WareConfigurator : public QFrame {
  Q_OBJECT

public:
  /**
   * Ctor
   * @param ware_id The ware to configure
   * @param parent The parent widget
   * @param is_secondary Is this a secondary ware (byproduct of the main
   * production)
   */
  explicit WareConfigurator(WareTarget *ware_target, QWidget *parent = nullptr);

  ~WareConfigurator() override;

  /**
   * Get the current ware target configuration
   * @return The ware target
   */
  const WareTarget *getWareTarget() const;

signals:
  /**
   * Signal emitted when the user wants to remove this ware from the list
   * @param ware_id
   */
  void shouldRemove(t_ware_id ware_id);

  /**
   * Signal emitted when the user changes the target or the source module
   */
  void shouldUpdate();

private:
  Ui::WareConfigurator *ui;
  WareTarget *ware_target;
};

#endif // WARECONFIGURATOR_HPP
