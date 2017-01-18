#include "../item.h"
#include "test_utils.h"

static const double INIT_CP = 4.5;
static const int INIT_HP = 4;

using namespace mtm::pokemongo;

bool testCandy() {
	// negative level
	ASSERT_THROW(ItemInvalidArgException, Candy(-1));

	// Valid usage
	Pokemon pokemon("pikachu", INIT_CP, INIT_HP);
	Candy candy(4);

	ASSERT_NO_THROW(candy.Use(pokemon));
	return true;
}

bool testPotion() {
	// negative level
	ASSERT_THROW(ItemInvalidArgException, Potion(-1));

	// Valid usage
	Pokemon pokemon("pikachu", INIT_CP, INIT_HP);
	Potion potion(4);

	ASSERT_NO_THROW(potion.Use(pokemon));
	return true;
}

int main() {
	RUN_TEST(testCandy);
	RUN_TEST(testPotion);

	return 0;
}
