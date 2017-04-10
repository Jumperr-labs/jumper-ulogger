from __future__ import print_function
import os
from subprocess import call
from jumper_sdk import jumper

PROJECT_DIR = os.path.dirname(__file__)
BINARY = os.path.join(PROJECT_DIR, '_build', 'nrf52832_xxaa.bin')


class uLogger:
    def __init__(self):
        client = jumper.Client()
        self.system = client.load_system()
        print('Connected to system')
        self.nrf = self.system.endpoints[0].nrf52

    @staticmethod
    def build():
        print('Building project binaries')
        call('make')

    def stop(self):
        print('Turning the device off')
        self.nrf.stop()

    def start(self):
        print('Turning the device on')
        self.nrf.start_with_binary(BINARY)
        print('Device is on')


def main():
    system = uLogger()
    system.stop()
    system.build()
    system.start()
    try:
        while True:
            pass
    except KeyboardInterrupt:
        system.stop()

if __name__ == '__main__':
    main()
