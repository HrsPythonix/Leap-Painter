#pragma once
#if !defined(LEAP_CLASS_H_)
#define LEAP_CLASS_H_
#include <iostream>
#include <cstring>
#include "Leap.h"

using std::cout;
using std::endl;
using namespace Leap;

/* Global variables*/

//flag
bool flag = true;

//current position
int cur_pos[2];//保存X,Y坐标
int switcher = 0;//Leap画画的开关
int leapmode = 0;//Leap模式的开关

class Leap_Listener : public Listener
{
public:
	virtual void onInit(const Controller&);
	virtual void onConnect(const Controller&);
	virtual void onDisconnect(const Controller&);
	virtual void onExit(const Controller&);
	virtual void onFrame(const Controller&);
	virtual void onFocusGained(const Controller&);
	virtual void onFocusLost(const Controller&);
	virtual void onDeviceChange(const Controller&);
	virtual void onServiceConnect(const Controller&);
	virtual void onServiceDisconnect(const Controller&);

private:
};

const std::string fingerNames[] = { "Thumb", "Index", "Middle", "Ring", "Pinky" };
const std::string boneNames[] = { "Metacarpal", "Proximal", "Middle", "Distal" };
const std::string stateNames[] = { "STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END" };

void Leap_Listener::onInit(const Controller& controller)
{
	cout << "Initialized" << endl;
}

void Leap_Listener::onConnect(const Controller& controller)//开启leap手势识别
{
	cout << "Connected" << endl;
	controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.enableGesture(Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Gesture::TYPE_SWIPE);
}

void Leap_Listener::onDisconnect(const Controller& controller)
{
	cout << "Disconnected" << endl;
}

void Leap_Listener::onExit(const Controller& controller)
{
	cout << "Exited" << endl;
}

void Leap_Listener::onFrame(const Controller& controller)
{

	const Frame frame = controller.frame();
	//对手势识别并进行处理
	const GestureList gestures = frame.gestures();
	for (int g = 0; g < gestures.count(); ++g) {
		Gesture gesture = gestures[g];
		//实现点击屏幕手势退出
		if (gesture.type() == Gesture::TYPE_SCREEN_TAP) {
			switcher = 0;break;
		}
	}
	HandList hands = frame.hands();
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl)
	{
	
			const Hand hand = *hl;
			std::string handType = hand.isLeft() ? "Left hand" : "Right hand";

			const Vector normal = hand.palmNormal();
			const Vector direction = hand.direction();
			// Get fingers
			const FingerList fingers = hand.fingers();

			for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl)
			{
				const Finger finger = *fl;
				if (fingerNames[finger.type()] == "Index")
				{
					flag = false;//初始化成功
					Bone::Type boneType = static_cast<Bone::Type>(3);
					Bone bone = finger.bone(boneType);
					//将手指的坐标传递
					cur_pos[0] =(int) 500 + 3 * bone.nextJoint()[0];
					cur_pos[1] =(int) 800 - 3 * bone.nextJoint()[1];
				}
			}
	}
	return;
}

void Leap_Listener::onFocusGained(const Controller& controller)
{
	cout << "Focus Gained" << endl;
}

void Leap_Listener::onFocusLost(const Controller& controller)
{
	cout << "Focus Lost" << endl;
}

void Leap_Listener::onDeviceChange(const Controller& controller)
{
	cout << "Device Changed" << endl;
	const DeviceList devices = controller.devices();

	for (int i = 0; i < devices.count(); ++i) {
		cout << "id: " << devices[i].toString() << endl;
		cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << endl;
	}
}

void Leap_Listener::onServiceConnect(const Controller& controller)
{
	cout << "Service Connected" << endl;
}

void Leap_Listener::onServiceDisconnect(const Controller& controller)
{
	cout << "Service Disconnected" << endl;
}

#endif