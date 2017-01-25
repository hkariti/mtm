/* Question 1 */
template<typename T, class Iterator> 
std::set<T> runAlg(Iterator begin, Iterator end,
	container_operator<Iterator> cmp) {

    typename std::set<T> filtered;
    for (Iterator it = begin; it != container.end(); it++) {
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
