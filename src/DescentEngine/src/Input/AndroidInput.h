#pragma once

#include <boost/noncopyable.hpp>

#include "../Cpp11.h"
#include "InputSystemBase.h"
#include "InputContainer.h"
#include "../Visuals/ScreenTransform.h"

#include <utility>

class AndroidInput: public InputSystemBase {
public:

	struct FingerLocation {

		FingerLocation(int id, float x, float y) :
				xpos(x), ypos(y), PointerId(id) {

		}

		float xpos;
		float ypos;
		int PointerId;
	};

	typedef std::vector<FingerLocation> FingerLocationList;

	void updateContainer(float deltaT) override {

		// reset state of button, will be set by the inject calls below
		auto & ipC = this->getContainer(0);
		ipC.resetKeypress();
	}

	// Finger has touched down for the first time on the touch screen
	void injectTouchDown(int pointer_id, float xpos, float ypos);

	// One finger moved on the touch screen. This function takes the
	// position of all fingers, because Android does not supply the
	// id of the finger which actually moved, but only the position of the
	// pointer finger
	void injectTouchMove(FingerLocationList locationList);

	// Finger was lifted from the touch screen
	void injectTouchUp(int pointer_id);

	void setScreenTransform(ScreenTransform st) {
		m_screenTransform = st;
	}

	Vector2 getDpadPosition() {
		computeVirtualControlsPositions();
		return m_posDpad;
	}

	Vector2 getButton1Position() {
		computeVirtualControlsPositions();
		return m_posButton1;
	}

	Vector2 getButton2Position() {
		computeVirtualControlsPositions();
		return m_posButton2;
	}

	size_t maxInputDevices() const {
		// 4 gamepads ...
		return 4;
	}

	void computeVirtualControlsPositions();

	DeviceList availableInputDevices() const;
private:

	// android touch input is y = 0 in the upper corner, this programs convention
	// is to have y = 0 in the lower corner
	float upsideDownY(float) const;

	std::pair<bool, Vector2> distanceToDPadCenter(float xpos, float ypos) const;

	std::pair<bool, Vector2> distanceToButton1Center(float xpos,
			float ypos) const;

	std::pair<bool, Vector2> distanceToButton2Center(float xpos,
			float ypos) const;

	std::pair<bool, Vector2> distanceHelper(float xpos, float ypos,
			Vector2 center, float radius) const;

	//std::vector<std::pair<InputSystemBase::ContainerId, int>> m_lastKeyPresses;
	std::vector<int> m_registeredDevices;

	// needed for the virtual dpads
	Vector2 m_posDpad;
	Vector2 m_posButton1;
	Vector2 m_posButton2;

	Vector2 m_padCenterInPixels;
	Vector2 m_padButton1InPixels;
	Vector2 m_padButton2InPixels;

	float m_padDpadRadiusInPixels = 0.0f;
	float m_padButtonRadiusInPixels = 0.0f;

	bool padPositionsSet = false;
	bool PadFingerSet = false;
	int PadFingerId = 0;

	ScreenTransform m_screenTransform;
};
