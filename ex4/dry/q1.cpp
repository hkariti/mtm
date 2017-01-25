#include <vector>
#include <set>
#include <iostream>

/* Question 1 */
template <class Iterator>
class ContainerOperator {
    public:
    virtual ~ContainerOperator() {};
    virtual bool operator()(const Iterator& begin1, Iterator const& end1,
                        Iterator const& begin2, Iterator const& end2) const = 0;
};

template<typename T, class Iterator> 
std::set<T> runAlg(Iterator begin, Iterator end,
	ContainerOperator<Iterator>& op) {

    std::set<T> filtered;
    for (Iterator it = begin; it != end; it++) {
        if (op(begin, it, it, end)) {
            filtered.insert(*it);
        }
    }
    return filtered;
}

// Question 2
template <class Iterator>
class IsLarger : public ContainerOperator<Iterator> {
    public:
    ~IsLarger() {};
    virtual bool operator()(const Iterator& begin1, Iterator const& end1,
                        Iterator const& begin2, Iterator const& end2) const {
        if (begin1 == end1) return false;
        Iterator current, next;
        current = next = begin1;
        next++;
        for (; next != end1; next++, current++); // Find the last item
        return *current > *begin2;
    }
};

bool is_sorted(std::vector<int>& array) {
    IsLarger<std::vector<int>::iterator> op;
    std::set<int> filtered;

    filtered = runAlg<int, std::vector<int>::iterator>(array.begin(),
                                                       array.end(), op);
    return filtered.empty();
}

using namespace std;
int main() {
    vector<int> vec;
    for (int i = 0; i < 10; i++) {
        vec.push_back(i);
    }
    vec.push_back(1);

    if (is_sorted(vec)) {
        cout << "sorted" << endl;
    } else {
        cout << "unsorted" ;
        cout << endl;
    }
    return 0;
}
