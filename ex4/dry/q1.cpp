/* Question 1 */
template<typename T, typename Iter> std::set<T> runAlg(Iter begin, Iter end, oper<Iter> cmp) {
    Iter it = begin;
    typename set::set<T> filtered;
    for (; it != container.end(); it++) {
        if (cmp(begin, it, it, end)) {
            filtered.insert(*it);
        }
    }
    return filtered;
}

/*
 * Question 2
 *
 * The operator compares the last element from its first argument with the first element of its second argument,
 * and returns true if the first is LARGER than the second (empty collection for first arg returns true). Upon run of the above algorithm, if the array
 * is indeed sorted smallest-to-largest, the returning set will be empty.
 */
