#pragma once

struct MouseButton
{
	enum {
		Left = 0,
		Right = 1,
		Middle = 2,
		Count = 3,
	};
};

struct MouseMask
{
	enum {
		Left = 0x01,
		Right = 0x02,
		Middle = 0x04,
		Wheel = 0x08,
	};
};

struct KeyMask
{
	enum {
		WasDown = 0x80,
		IsDown  = 0x01
	};
};