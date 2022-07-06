import socket
import sys

from enum import IntEnum, auto

from PyQt5.QtCore import pyqtSlot
from PyQt5.QtGui import QCloseEvent, QColor
from PyQt5.QtWidgets import QApplication, QColorDialog, QFrame, QMainWindow
from PyQt5.uic import loadUi


HOST = "effectlight.local"
PORT = 8888

UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

#UDPServerSocket.sendto(bytearray([3, 0, 0, 0, 9, 0]), (HOST, PORT))
#UDPServerSocket.sendto(bytearray([1]), (HOST, PORT))


class MessageHeader(IntEnum):
    Clear = 0
    Show = auto()
    UpdateLength = auto()
    Set = auto()
    Fill = auto()
    RunProgram = 100
    StopProgram = auto()


def split_qcolor(color: QColor) -> list[int]:
    return [color.red(), color.green(), color.blue()]


class MainWindow(QMainWindow):
    def __init__(self, parent=None):
        super().__init__(parent)

        loadUi("mainWindow.ui", self)
        self.setFixedSize(self.size())

        self.color_dialog: QColorDialog
        self.color_dialog = QColorDialog()
        self.color_dialog.currentColorChanged.connect(self.colorSelected)
        self.color_dialog.show()

        self.color_indicator: QFrame
        self.color_indicator = self.findChild(QFrame, "colorIndicator")

        #UDPServerSocket.sendto(bytearray([MessageHeader.StopProgram]), (HOST, PORT))

    def closeEvent(self, event: QCloseEvent) -> None:
        self.color_dialog.close()
        return super().closeEvent(event)

    def showColorPicker(self):
        self.color_dialog.show()

    def setPanel1(self):
        start = [0, 0]
        count = [0, 48]
        rgb = split_qcolor(self.color_dialog.currentColor())
        message = [MessageHeader.Fill] + start + count + rgb
        UDPServerSocket.sendto(bytearray(message), (HOST, PORT))
        UDPServerSocket.sendto(bytearray([MessageHeader.Show]), (HOST, PORT))

    def setPanel2(self):
        start = [0, 48]
        count = [0, 48]
        rgb = split_qcolor(self.color_dialog.currentColor())
        message = [MessageHeader.Fill] + start + count + rgb
        UDPServerSocket.sendto(bytearray(message), (HOST, PORT))
        UDPServerSocket.sendto(bytearray([MessageHeader.Show]), (HOST, PORT))

    @pyqtSlot()
    def colorSelected(self):
        color = self.color_dialog.currentColor()
        self.color_indicator.setStyleSheet(f"background-color: rgb({color.red()},{color.green()},{color.blue()}); border: 1px solid black;")


app = QApplication(sys.argv)
app.setStyle("Fusion")
win = MainWindow()
win.show()
app.exec()
