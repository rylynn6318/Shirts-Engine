#include "core/input/Input.h"
#include "GLFW/glfw3.h"

auto se::Input::saveKeymap(se::Key key, se::ButtonState state) -> void {
	// std::map []로 접근하면, 키가 없으면 만들고 있으면 그걸 씀
	// https://en.cppreference.com/w/cpp/container/map/operator_at
	auto& key_state = keymap[key];
	if (state == se::ButtonState::DOWN) {
		switch (key_state) {
		case se::KeyState::DEFAULT:
		case se::KeyState::RELEASED:
			key_state = se::KeyState::PRESSED;
			break;
		case se::KeyState::PRESSED:
		case se::KeyState::HELD_DOWN:
			key_state = se::KeyState::HELD_DOWN;
			break;
		}
	}
	else if (state == se::ButtonState::UP) {
		switch (key_state) {
		case se::KeyState::DEFAULT:
		case se::KeyState::RELEASED:
			key_state = se::KeyState::DEFAULT;
			break;
		case se::KeyState::PRESSED:
		case se::KeyState::HELD_DOWN:
			key_state = se::KeyState::RELEASED;
			break;
		}
	}
}

se::Input::Input() {
	for (auto key : se::Key()) {
		keymap[key] = se::KeyState::DEFAULT;
	}
}

auto se::Input::toSEKey(int keycode) -> se::Key {
	switch (keycode) {
	case GLFW_KEY_A:
		return se::Key::A;
	case GLFW_KEY_B:
		return se::Key::B;
	case GLFW_KEY_C:
		return se::Key::C;
	case GLFW_KEY_D:
		return se::Key::D;
	case GLFW_KEY_E:
		return se::Key::E;
	case GLFW_KEY_F:
		return se::Key::F;
	case GLFW_KEY_G:
		return se::Key::G;
	case GLFW_KEY_H:
		return se::Key::H;
	case GLFW_KEY_I:
		return se::Key::I;
	case GLFW_KEY_J:
		return se::Key::J;
	case GLFW_KEY_K:
		return se::Key::K;
	case GLFW_KEY_L:
		return se::Key::L;
	case GLFW_KEY_M:
		return se::Key::M;
	case GLFW_KEY_N:
		return se::Key::N;
	case GLFW_KEY_O:
		return se::Key::O;
	case GLFW_KEY_P:
		return se::Key::P;
	case GLFW_KEY_Q:
		return se::Key::Q;
	case GLFW_KEY_R:
		return se::Key::R;
	case GLFW_KEY_S:
		return se::Key::S;
	case GLFW_KEY_T:
		return se::Key::T;
	case GLFW_KEY_U:
		return se::Key::U;
	case GLFW_KEY_V:
		return se::Key::V;
	case GLFW_KEY_W:
		return se::Key::W;
	case GLFW_KEY_X:
		return se::Key::X;
	case GLFW_KEY_Y:
		return se::Key::Y;
	case GLFW_KEY_Z:
		return se::Key::Z;
	case GLFW_KEY_ESCAPE:
		return se::Key::ESC;
	default:
		return se::Key::NONE;
	}
}

auto se::Input::toButtonState(int state) -> se::ButtonState {
	if (state)
		return se::ButtonState::DOWN;
	else
		return se::ButtonState::UP;
}

// TODO : 이런게 SDK_INPUT 필요없는건 Input.cpp 만들어서 거기 둬야할 듯
auto se::Input::checkKey(se::KeyState state, se::Key key) -> bool {
	return keymap.at(key) == state;
}

template<typename... Keys>
auto se::Input::checkKey(se::KeyState state, se::Key key, Keys... keys) -> bool {
	return checkKey(state, key) && checkKey(state, keys...);
}

auto se::Input::saveKeymap(int keycode, int state) -> void {
	saveKeymap(toSEKey(keycode), toButtonState(state));
}

auto se::Input::pollKeyEvent() -> void {

}

auto se::Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)->void
{
	saveKeymap(key, action);
}

auto se::Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)->void
{
	saveMouseButtonToKeymap(button);
}

auto se::Input::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)->void
{
	mouse_x = xpos;
	mouse_y = ypos;
}

auto se::Input::getMousePosition() -> std::pair<int, int> {
	return { mouse_x, mouse_y };
}

auto se::Input::saveMouseButtonToKeymap(int state) -> void {
	if (state & GLFW_MOUSE_BUTTON_LEFT)
		saveKeymap(Key::MOUSE_1, toButtonState(state));
	else if (state & GLFW_MOUSE_BUTTON_MIDDLE)
		saveKeymap(Key::MOUSE_2, toButtonState(state));
	else if (state & GLFW_MOUSE_BUTTON_RIGHT)
		saveKeymap(Key::MOUSE_3, toButtonState(state));
	//else if (state & SDL_BUTTON(SDL_BUTTON_X1))
	//	saveKeymap(Key::MOUSE_4, toButtonState(state));
	//else if (state & SDL_BUTTON(SDL_BUTTON_X2))
	//	saveKeymap(Key::MOUSE_5, toButtonState(state));
}