namespace detail {

// The implementation of this idea is taken out inside the 'details'
// namespace by a typical template struct predicate specialization.
//
template <typename t>
struct is_type_list : std::false_type {};
//
template <typename... types>
struct is_type_list<type_list<types...>> : std::true_type {};

//
//
template <typename list, template <typename> typename constraint>
struct is_constrained_type_list : std::false_type {};
//
template <template <typename> typename constraint, typename... types>
struct is_constrained_type_list<type_list<types...>, constraint> {
  static constexpr bool value = (constraint<types>::value && ...);
};

}  // namespace detail
