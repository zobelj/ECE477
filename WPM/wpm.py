#!/bin/python3

import keyboard
import time
from datetime import datetime
from threading import Timer

class WPMCalculator:
    def __init__(self, interval):
        self.interval = interval
        self.timer = Timer(interval, self.report)
        self.intervalCharCount = 0
        self.charCount = 0
        self.startTime = 0

    def callBack(self, event):
        if self.charCount == 0:
            self.startTime = datetime.now()
            self.timer.start()
        name = event.name
        self.charCount += 1
        self.intervalCharCount += 1

    def calcAvg(self):
        deltaTime = datetime.now() - self.startTime()
        return self.charCount / 5 / deltaTime

    def report(self):
        if (self.intervalCharCount == 0):
            self.timer.cancel()
        print(f'Words per minute: {self.calcAvg}')
        self.intervalCharCount = 0

    def start(self):
        keyboard.on_release(callback=self.callBack)

if __name__ == "__main__":
    calculator = WPMCalculator(3)
    calculator.start()
