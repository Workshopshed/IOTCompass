using FunctionApp;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace TestFunction {
    [TestClass]
    public class TestFormatting {
        [TestMethod]
        public void TestFormat() {
            var source = new Coords(51.5074, 0.0);
            var dest = new Coords(55.7558, 37.6173);
            double direction = 45.43;
            dynamic result = LocationToDirection.ResultsToJObject(source, dest, direction);

            Assert.IsNotNull(result);
            Assert.AreEqual(source.lat,(double)result.Source.lat);
            Assert.AreEqual(source.lon, (double)result.Source.lon);
            Assert.AreEqual(dest.lat, (double)result.Destination.lat);
            Assert.AreEqual(dest.lon, (double)result.Destination.lon);
            Assert.AreEqual(direction, (double)result.Direction);
        }
    }
}