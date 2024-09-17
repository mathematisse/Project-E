#define BOOST_TEST_MODULE ECSTest
#include <boost/test/included/unit_test.hpp>
#include "ecs/ecs.hpp"

BOOST_AUTO_TEST_CASE(test_max_entities)
{
    BOOST_CHECK_EQUAL(MAX_ENTITIES, 32);
    BOOST_CHECK(false);
}