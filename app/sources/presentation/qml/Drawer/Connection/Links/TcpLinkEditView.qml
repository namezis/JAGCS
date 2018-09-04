import QtQuick 2.6
import QtQuick.Layouts 1.3
import JAGCS 1.0

import Industrial.Controls 1.0 as Controls

BaseLinkEditView {
    id: linkView

    Controls.TextField {
        labelText: qsTr("Address")
        text: provider.parameter(LinkDescription.Address)
        onEditingFinished: provider.setParameter(LinkDescription.Address, value)
        Layout.fillWidth: true
    }

    Controls.SpinBox {
        labelText: qsTr("Port")
        from: 0
        to: 65535
        value: provider.parameter(LinkDescription.Port)
        onValueModified: provider.setParameter(LinkDescription.Port, value)
        Layout.fillWidth: true
    }
}
