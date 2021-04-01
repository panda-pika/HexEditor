#include "cheats.hpp"
#include "Helpers.hpp"
#include "CTRPluginFramework.hpp"


namespace CTRPluginFramework
{
	void HexEditor(MenuEntry* entry)
	{

		static u32 Address = 0x0847b2f4;
		u32 Value;
		static int posX = 0, posY = 0;

		static bool doubleclick = false;
		static bool countcheck = false;
		static int ReadCount = 0;

		if (Controller::IsKeyPressed(ZL))countcheck = !countcheck;
		if (countcheck) ReadCount++;
		if (!countcheck && ReadCount > 0 && ReadCount < 8)
		{
			doubleclick = !doubleclick;
			ReadCount = 0;
		}

		if (ReadCount > 7)
		{
			ReadCount = 0;
			countcheck = false;
		}

		if (doubleclick)
		{
			Process::Read32((Address + posX * 0x4 + posY * 0x10), Value);

			if (Controller::IsKeyDown(ZL))
			{
				{
					if (Controller::IsKeyPressed(DPadUp) && posY > 0) posY--;
					if (Controller::IsKeyPressed(DPadDown) && posY < 9)posY++;
					if (Controller::IsKeyPressed(DPadLeft) && posX > 0)posX--;
					if (Controller::IsKeyPressed(DPadRight) && posX < 3)posX++;
				}

				if (Controller::IsKeyPressed(A))
				{
					Keyboard key("値を編集");
					key.Open(Value, Value);
					Process::Write32((Address + posX * 0x4 + posY * 0x10), Value);
				}

				if (Controller::IsKeyPressed(Y))
				{
					Keyboard key("ジャンプ");
					key.Open(Address, Address);
				}

				{
					if (Controller::IsKeyPressed(DPadUp) && posY == 0) Address -= 0x10;
					if (Controller::IsKeyPressed(DPadDown) && posY == 9) Address += 0x10;
				}
			}

			const Screen screen = OSD::GetTopScreen();
			for (int i = 0; i < 10; i++)
			{
				screen.Draw(Utils::Format("%08X", Address + posX * 0x4 + posY * 0x10), 0, 0, Color::Black, Color::SkyBlue);

				screen.Draw(Utils::Format("%08X", Address + i * 0x10), 0, (i * 10 + 11), Color::Black, Color::SkyBlue);

				for (int I = 0; I < 4; I++)
				{
					screen.Draw(Utils::Format("%08X", (Address + 4 * I) & 0xF), (6 * 8 + 3) + ((6 * 8 + 3) * I), 0, Color::Black, Color::DeepSkyBlue);

					Color BackColor = Color::White;

					if (I == posX && i == posY) BackColor = Color::DeepSkyBlue;

					if (Process::Read32((Address + i * 10) + (I * 0x4), Value))
						screen.Draw(Utils::Format("%08X", Value), (6 * 8 + 3) + ((6 * 8 + 3) * I), i * 10 + 11, Color::Black, BackColor);
					else
						screen.Draw("????????", (6 * 8 + 3) + ((6 * 8 + 3) * I), i * 10 + 11, Color::Black, BackColor);


				}
			}
		}
	}
}
