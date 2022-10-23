namespace detail {

// The implementation of this idea is taken out inside the 'details'
// namespace by a typical template struct predicate specialization.
//
template <typename t>
struct is_value_list : std::false_type {};
//
template <auto... values>
struct is_value_list<value_list<values...>> : std::true_type {};

//
//
template <typename list, template <auto> typename constraint>
struct is_constrained_value_list : std::false_type {};
//
template <template <auto> typename constraint, auto... values>
struct is_constrained_value_list<value_list<values...>, constraint> {
  static constexpr bool value = (constraint<values>::value && ...);
};

// Checks, whether the type is an instance of the 'value_list' template
// that contains only values with the given type.
//
template <typename list, typename type>
struct is_uniform_value_list : std::false_type {};
//
template <typename type, type... values>
struct is_uniform_value_list<value_list<values...>, type> : std::true_type {};

}  // namespace detail
