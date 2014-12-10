
#include "devmain.h"
#include "dosbox.h"

using namespace dosbox;

void Delay(uint32_t nTime) {
	for( ; nTime != 0; nTime--) {
		__NOP();
	}
}

int main(void)
{
	devmain();

	CDosBox* dos = new CDosBox();

	dos->Execute();

	while (1) {
		GPIOA->ODR ^= GPIO_PIN_8;
		Delay(2000000);
		__NOP();
	}

	delete dos;

	return 0;
}
