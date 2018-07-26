
.pragma library


var internalQmlObject = Qt.createQmlObject('import QtQuick 2.0; QtObject { signal someSignal() }', Qt.application, 'InternalQmlObject');

function doSomething() {
    internalQmlObject.someSignal();
}


