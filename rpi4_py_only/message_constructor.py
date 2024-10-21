############################################
## Author: Michal Balogh, xbalog06        ##
## Date: 14.10.2024                       ##
############################################

from constants import *

class MessageConstructor:
    @staticmethod
    def init_message(address, command):
        message = [0x00] * TOTAL_MESSAGE_SIZE
        message[0] = HEADER
        message[1] = command
        message[2] = address
        message[-1] = FOOTER
        return bytearray(message)

    @staticmethod
    def set_symbol(message, symbol, pos, line):
        if pos < 0 or pos >= DATA_SIZE or line < 0 or line >= NUM_LINES:
            return
        segments_symbol = MessageConstructor.get_segments_from_letter(symbol)
        if isinstance(segments_symbol, str):
            segments_symbol = ord(segments_symbol)
        message[pos + line * SYMBOLS_PER_LINE + DATA_OFFSET] = segments_symbol

    @staticmethod
    def display_clear(address, command, serial_communication):
        message_clear = MessageConstructor.init_message(address, command)
        if message_clear is None:
            return

        for i in range(DATA_SIZE):
            message_clear[i + DATA_OFFSET] = SPACE

        bytes_sent = serial_communication.send(message_clear)
        if bytes_sent != TOTAL_MESSAGE_SIZE:
            return

    @staticmethod
    def get_segments_from_digit(digit):
        return {
            0: ZERO,
            1: ONE,
            2: TWO,
            3: THREE,
            4: FOUR,
            5: FIVE,
            6: SIX,
            7: SEVEN,
            8: EIGHT,
            9: NINE
        }.get(digit, 0x00)

    @staticmethod
    def get_segments_from_letter(letter):
        return {
            'A': A, 'B': B, 'C': C, 'D': D, 'E': E, 'F': F, 'G': G,
            'H': H, 'I': I, 'J': J, 'K': K, 'L': L, 'M': M, 'N': N,
            'O': O, 'P': P, 'Q': Q, 'R': R, 'S': S, 'T': T, 'U': U,
            'V': V, 'W': W, 'X': X, 'Y': Y, 'Z': Z
        }.get(letter, 0x00)
