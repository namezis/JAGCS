import QtQuick 2.6
import QtQuick.Layouts 1.3

import Industrial.Controls 1.0 as Controls

Controls.Frame {
    id: root

    property string endpoints

    property var _endpoints: []

    signal changed(string endpoints)

    function updateEndpoints(update) {
        list.model = _endpoints;
        if (update) changed(_endpoints.join(";"));
    }

    onEndpointsChanged: {
        _endpoints = endpoints.split(";");
        updateEndpoints(false);
    }

    padding: sizings.padding
    implicitHeight: list.contentHeight + padding * 2
    backgroundColor: customPalette.sunkenColor

    ListView {
        id: list
        anchors.fill: parent
        flickableDirection: Flickable.AutoFlickIfNeeded
        boundsBehavior: Flickable.StopAtBounds
        spacing: sizings.spacing
        footerPositioning: ListView.OverlayFooter
        model: _endpoints
        clip: true

        Controls.ScrollBar.vertical: Controls.ScrollBar {
            visible: parent.contentHeight > parent.height
        }

        function toBottom() {
            contentY = contentHeight - height;
        }

        header: RowLayout {
            width: parent.width
            spacing: sizings.spacing

            Controls.Label {
                text: qsTr("Address")
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            Controls.Label {
                text: qsTr("Port")
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }
        }

        delegate: EndpointView {
            width: parent.width
            endpoint: modelData
            onRemove: {
                _endpoints.splice(index, 1);
                updateEndpoints(true);
            }
            onChanged:{
                _endpoints[index] = endpoint;
                updateEndpoints(true);
            }
        }

        footer: Controls.Button {
            width: parent.width
            text: qsTr("Add endpoint")
            iconSource: "qrc:/ui/plus.svg"
            z: 10
            onClicked: {
                _endpoints.push("127.0.0.1:8080");
                updateEndpoints(true);
                list.toBottom();
            }
        }
    }
}
