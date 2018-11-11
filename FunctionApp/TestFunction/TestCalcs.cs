using FunctionApp;
using Microsoft.VisualStudio.TestTools.UnitTesting;

//Test URLs (can we put the calcs for these as a unit test?)
//London to Paris
//http://localhost:7071/api/LocationToDirection?lat1=51.5074&lon1=0.0&lat2=48.8566&lon2=2.3522
//London to Moscow
//http://localhost:7071/api/LocationToDirection?lat1=51.5074&lon1=0.0&lat2=55.7558&lon2=37.6173
//London to Seatle
//http://localhost:7071/api/LocationToDirection?lat1=51.5074&lon1=0.0&lat2=47.6062&lon2=122.3321

namespace TestFunction {
    [TestClass]
    public class TestCalcs {
        [TestMethod]
        public void TestCalcLondonMoscow() {
            var source = new Coords(51.5074, 0.0);
            var dest = new Coords(55.7558, 37.6173);
            var result = LocationToDirection.calcDirection(source, dest);
            Assert.AreEqual(83.8782195858749, result);
        }

        [TestMethod]
        public void TestNoWhere() {
            //Direction doesn't really matter as long as there are no overflows exceptions etc.
            var loc = new Coords(10.123, 10.123);
            var result = LocationToDirection.calcDirection(loc, loc);
            Assert.AreEqual(360, result);
        }
    }
}
