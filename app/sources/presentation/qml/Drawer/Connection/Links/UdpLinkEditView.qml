import QtQuick 2.6
import QtQuick.Layouts 1.3
import JAGCS 1.0

import Industrial.Controls 1.0 as Controls

BaseLinkEditView {
    id: linkView

    Controls.SpinBox {
        labelText: qsTr("Port")
        from: 0
        to: 65535
        value: provider.parameter(LinkDescription.Port)
        onValueModified: provider.setParameter(LinkDescription.Port, value)
        Layout.fillWidth: true
    }

    Controls.Label {
        text: qsTr("Setted endpoints")
        horizontalAlignment: Text.AlignHCenter
        Layout.fillWidth: true
    }

    EndpointListView {
        endpoints: provider.parameter(LinkDescription.Endpoints)
        onChanged: provider.setParameter(LinkDescription.Endpoints, endpoints)
        Layout.maximumHeight: sizings.controlBaseSize * 6
        Layout.fillWidth: true
    }

    Controls.CheckBox {
        text: qsTr("Add endpoint on recv")
        horizontalAlignment: Text.AlignHCenter
        checked: provider.parameter(LinkDescription.UdpAutoResponse)
        onToggled: provider.setParameter(LinkDescription.UdpAutoResponse, toggled)
        Layout.fillWidth: true
    }
}
