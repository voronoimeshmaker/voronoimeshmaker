/**
 * @file GeometricDataHolder.tpp
 * @brief Template implementations for the GeometricDataHolder class.
 * This file contains the template function implementations for the GeometricDataHolder class.
 *
 * @ingroup parameters
 * 
 * This file defines the methods used to add and retrieve values from the GeometricDataHolder class.
 * It provides template specializations for the supported data types.
 * 
 * @version 1.0
 * @date 2024
 */

VORMAKER_NAMESPACE_OPEN

    template<typename T>
    void GeometricDataHolder::addValue(const std::string& key, T value) {
        data_[key] = value;
        std::cout << "Added value of type: " << GeometricDataTraits<T>::name() << std::endl;
    }

    template<typename T>
    std::optional<T> GeometricDataHolder::getValue(const std::string& key) const {
        auto it = data_.find(key);
        if (it != data_.end()) {
            if (auto pval = std::get_if<T>(&(it->second))) {
                std::cout << "Retrieved value of type: " << GeometricDataTraits<T>::name() << std::endl;
                return *pval;
            }
        }
        return std::nullopt;  // Return nullopt if the type or key is invalid
    }

    std::ostream& operator<<(std::ostream& os, const GeometricDataHolder& holder) {
        for (const auto& [key, value] : holder.data_) {
            os << key << ": ";
            std::visit([&os](auto&& arg) {
                os << arg;
            }, value);
            os << std::endl;
        }
        return os;
    }

    void GeometricDataHolder::printTypes() const {
        for (const auto& [key, value] : data_) {
            std::cout << key << ": ";
            std::visit([](auto&& arg) {
                std::cout << "Stored type: " << GeometricDataTraits<std::decay_t<decltype(arg)>>::name() << std::endl;
            }, value);
        }
    }

VORMAKER_NAMESPACE_CLOSE
