/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** InputPlayer
*/

#ifndef INPUTPLAYER_HPP
    #define INPUTPLAYER_HPP
#include "Component.hpp"
#include <cstddef>

namespace ecs {
	/**
	 * @Class InputPlayer
	 * @brief Represents a player that handles input for a specified system or game
	 */
	class InputPlayer : public Component {
		/**
		 *
		 */
	public:
	    InputPlayer()
	    {
	        _id += 1;
	    };
		/**
		 * @brief Destructor for the InputPlayer class
		 * This is the default override of the destructor for the InputPlayer class,
		 * which ensures proper destruction of the object and adheres to the
		 * Component interface's virtual destructor requirements
		 */
		~InputPlayer() override = default;

		/**
		 * @brief Retrieves the state of the "up" input
		 * This method checks whether the "up" key or input is currently active and
		 * returns its state. It provides information on whether the corresponding
		 * key or action has been triggered
		 * @return A boolean value representing the state of the "up" input
		 * Returns `true` if the "up" input is active (key pressed), or `false` otherwise
		 */
		[[nodiscard]] bool getUp() const;

		/**
		 * @brief Checks the state of the "down" input
		 * This method retrieves the current state of the "down" input to determine
		 * whether the corresponding key or action is pressed. It returns a boolean
		 * value indicating the input's state
		 * @return A boolean value representing the state of the "down" input
		 * Returns `true` if the "down" input is active (key pressed), or `false` otherwise
		 */
		[[nodiscard]] bool getDown() const;

		/**
		 * @brief Retrieves the state of the left input
		 * This method checks whether the "left" key or input is currently active. It
		 * retrieves the state and returns a boolean value indicating whether the
		 * input is pressed or not
		 * @return A boolean value representing the state of the left input
		 * Returns `true` if the "left" input is active (key pressed), or `false` otherwise
		 */
		[[nodiscard]] bool getLeft() const;

		/**
		 * @brief Checks the state of the "right" input
		 * This method retrieves the current state of the "right" input to determine
		 * whether the corresponding key or action is pressed. It returns a boolean
		 * value indicating the input's state
		 * @return A boolean value representing the state of the "right" input
		 * Returns `true` if the "right" input is active (key is pressed), or `false`
		 * if it is not
		 */
		[[nodiscard]] bool getRight() const;

		/**
		 * @brief Checks the state of the spacebar input
		 * This method retrieves the current state of the spacebar input to determine
		 * whether the spacebar key is pressed or not. It returns a boolean value
		 * representing this state
		 * @return A boolean value indicating the state of the spacebar input
		 * Returns `true` if the spacebar is pressed, or `false` if it is not
		 */
		[[nodiscard]] bool getSpacebar() const;

		/**
		 * @brief Sets the state of the "up" input
		 * Updates the state of the "up" input, indicating whether the corresponding key
		 * is currently pressed or not
		 * @param up A boolean value representing the state of the "up" input
		 * Pass `true` if the input is active (key is pressed), or `false` if inactive
		 */
		void setUp(bool up);

		/**
		 * @brief Sets the state of the "down" input
		 * Updates the state of the "down" input, indicating whether the corresponding key
		 * is currently pressed or not
		 * @param down A boolean value representing the state of the "down" input
		 * Pass `true` if the input is active (key is pressed), or `false` if inactive
		 */
		void setDown(bool down);

		/**
		 * @brief Sets the state of the left input
		 * Updates the state of the left input to indicate whether the corresponding key
		 * is pressed or not
		 * @param left A boolean value representing the state of the left input
		 * Pass `true` if the left input is active (key pressed), otherwise `false`
		 */
		void setLeft(bool left);

		/**
		 * @brief Sets the state of the right input
		 * This method updates the state of the right input, specifying whether the
		 * right key or action is pressed or not
		 * @param right A boolean value representing the state of the right input
		 * Pass `true` if the right input is active, or `false` otherwise
		 */
		void setRight(bool right);

		/**
		 * @brief Sets the state of the spacebar input
		 * This method updates the state of the spacebar input, indicating whether the key
		 * is pressed or not
		 * @param spacebar A boolean value representing the state of the spacebar input
		 * Pass `true` if the spacebar is pressed, or `false` otherwise.
		 */
		void setSpacebar(bool spacebar);

		/**
		 * @brief Retrieves the unique identifier of the InputPlayer
		 * This method returns the unique identifier assigned to the InputPlayer
		 * instance, which can be used for distinguishing between different
		 * InputPlayer objects within the system
		 * @return The unique identifier of the InputPlayer as a std::size_t
		 */
		[[nodiscard]] std::size_t getId() const;

		/**
		 * @brief Update Animation
		 */
		void updateAnimation(float dt);

		/**
		 * @brief Gets Animations frame
		 * @return int
		 */
		int getAnimFrame() const;

	private:
	    bool _up = false;
		bool _down = false;
		bool _left = false;
		bool _right = false;
		bool _spacebar = false;
		std::size_t _id = 0;
		int _animFrame = 0;
		float _animTimer = 0.f;
		float _animSpeed = 0.1f;
	};
}

#endif //INPUTPLAYER_HPP