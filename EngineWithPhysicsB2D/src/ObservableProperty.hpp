//This code was made for the Multimedia Project 2a, 
//in the Multimedia Technology class at the FH Salzburg, 
//by Christopher Kastner and Tim Paul
#pragma once

#include <functional>
#include <memory>
#include <vector>

namespace rd2d
{
/**
 * @brief Observable property that notifies listeners when the value changes.
 *
 * This is a reactive programming pattern commonly used in game engines (Unity, Unreal)
 * to automatically update UI or game state when properties change.
 *
 * **Example Usage:**
 * @code
 * ObservableProperty<int> coins{0};
 *
 * // Register callback
 * coins.onChange([](int newValue) {
 *     std::cout << "Coins changed to: " << newValue << std::endl;
 * });
 *
 * coins = 10;  // Triggers callback automatically
 * coins += 5;  // Triggers callback automatically
 * @endcode
 *
 * **Teaching Example:**
 * This class demonstrates the Observer pattern, which is one of the Gang of Four
 * design patterns. It's particularly useful for:
 * - UI updates when game state changes
 * - Automatic visual feedback
 * - Decoupling game logic from presentation
 * - Event-driven programming
 *
 * @tparam T The type of the observable value (must be copyable)
 */
template <typename T>
class ObservableProperty
{
public:
    using CallbackFunction = std::function<void(const T&)>;

    /**
     * @brief Construct with initial value
     * @param initialValue Starting value (default-constructed if not provided)
     */
    explicit ObservableProperty(T initialValue = T{}) : m_value(std::move(initialValue))
    {
    }

    /**
     * @brief Register a callback to be invoked when the value changes
     * @param callback Function to call when value changes (receives new value)
     * @return Callback ID for later removal (currently not used, but reserved for future)
     */
    size_t onChange(CallbackFunction callback)
    {
        m_callbacks.push_back(std::move(callback));
        return m_callbacks.size() - 1;
    }

    /**
     * @brief Set the value and notify all listeners
     * @param newValue The new value to set
     */
    void set(T newValue)
    {
        if (m_value != newValue)
        { // Only notify if value actually changed
            m_value = std::move(newValue);
            notifyListeners();
        }
    }

    /**
     * @brief Get the current value (const access)
     * @return Const reference to current value
     */
    const T& get() const
    {
        return m_value;
    }

    /**
     * @brief Set value silently without triggering callbacks
     *
     * Useful for initialization or bulk updates where you want to
     * manually trigger the update later.
     *
     * @param newValue The new value to set
     */
    void setSilent(T newValue)
    {
        m_value = std::move(newValue);
    }

    /**
     * @brief Manually trigger all callbacks with the current value
     *
     * Useful when you need to force an update, or after using setSilent().
     */
    void notifyListeners()
    {
        for (const auto& callback : m_callbacks)
        {
            callback(m_value);
        }
    }

    /**
     * @brief Clear all registered callbacks
     */
    void clearCallbacks()
    {
        m_callbacks.clear();
    }

    // Operator overloads for convenient syntax

    /**
     * @brief Assignment operator - sets value and notifies listeners
     */
    ObservableProperty& operator=(const T& newValue)
    {
        set(newValue);
        return *this;
    }

    /**
     * @brief Implicit conversion to T for reading the value
     */
    operator const T&() const
    {
        return m_value;
    }

    /**
     * @brief Addition assignment - modifies value and notifies listeners
     */
    ObservableProperty& operator+=(const T& rhs)
    {
        set(m_value + rhs);
        return *this;
    }

    /**
     * @brief Subtraction assignment - modifies value and notifies listeners
     */
    ObservableProperty& operator-=(const T& rhs)
    {
        set(m_value - rhs);
        return *this;
    }

    /**
     * @brief Multiplication assignment - modifies value and notifies listeners
     */
    ObservableProperty& operator*=(const T& rhs)
    {
        set(m_value * rhs);
        return *this;
    }

    /**
     * @brief Division assignment - modifies value and notifies listeners
     */
    ObservableProperty& operator/=(const T& rhs)
    {
        set(m_value / rhs);
        return *this;
    }

    /**
     * @brief Pre-increment operator
     */
    ObservableProperty& operator++()
    {
        set(m_value + 1);
        return *this;
    }

    /**
     * @brief Post-increment operator
     */
    T operator++(int)
    {
        T old = m_value;
        set(m_value + 1);
        return old;
    }

    /**
     * @brief Pre-decrement operator
     */
    ObservableProperty& operator--()
    {
        set(m_value - 1);
        return *this;
    }

    /**
     * @brief Post-decrement operator
     */
    T operator--(int)
    {
        T old = m_value;
        set(m_value - 1);
        return old;
    }

private:
    T                             m_value;
    std::vector<CallbackFunction> m_callbacks;
};
} // namespace rd2d
