#pragma once

#include <dev/CharacterDevice.hpp>

namespace arch {
    namespace i686 {
        class VGATextDevice : public CharacterDevice {
        private:
            static uint8_t* g_ScreenBuffer;
            int m_ScreenX;
            int m_ScreenY;

            void PutChar(char c);
            void PutChar(int x, int y, char c);
            void PutColor(int x, int y, uint8_t color);
            char GetChar(int x, int y);
            uint8_t GetColor(int x, int y);
            void SetCursor(int x, int y);
            void Scrollback(int lines);
        public:
            VGATextDevice();
            size_t Read(uint8_t* data, size_t size);
            size_t Write(const uint8_t* data, size_t size);
            void Clear();
        };
    }
}
