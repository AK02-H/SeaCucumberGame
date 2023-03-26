#include "UiButton.h"

void UiButton::ProcessMouseDownEvent(Vector2i mousePos) {

	
	if (!active)
	{
		cout << "BUTTON PRESSED, BUT BUTTON WAS INACTIVE" << endl;
		return;
	}

	float minX = sprite.getGlobalBounds().left;
	float maxX = sprite.getGlobalBounds().width + minX;
	float minY = sprite.getGlobalBounds().top;
	float maxY = sprite.getGlobalBounds().height + minY;

	float mouseX = mousePos.x;
	float mouseY = mousePos.y;

	bool isClicked = mouseX >= minX && mouseX <= maxX && mouseY >= minY && mouseY <= maxY;

	if (isClicked) {

		if (gameSystem != nullptr) {
			//Currently not used
		}

		if (feSystem != nullptr) {
			feSystem->ButtonPressEvent(feFuncId, additionalParameter_1);
		}

		
	}
}