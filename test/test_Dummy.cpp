#include <boost/test/unit_test.hpp>
#include <envire_gis/Dummy.hpp>

using namespace envire_gis;

BOOST_AUTO_TEST_CASE(it_should_not_crash_when_welcome_is_called)
{
    envire_gis::DummyClass dummy;
    dummy.welcome();
}
