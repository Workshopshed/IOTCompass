
using System.IO;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Extensions.Http;
using Microsoft.AspNetCore.Http;
using Microsoft.Azure.WebJobs.Host;
using Newtonsoft.Json.Linq;
using System;

namespace FunctionApp
{
    public static class LocationToDirection {
        [FunctionName("LocationToDirection")]
        public static IActionResult Run([HttpTrigger(AuthorizationLevel.Function, "get", "post", Route = null)]HttpRequest req, TraceWriter log) {

            log.Info($"LocationToDirection called");

            Coords source = new Coords();
            Coords dest = new Coords();

            var parseResult = parseParameters(req, out source, out dest);
            if (parseResult != null) return parseResult;

            double direction = calcDirection(source,dest);

            var returnObject = ResultsToJObject(source, dest, direction);

            return new JsonResult(returnObject);
        }

        public static JObject ResultsToJObject(Coords source, Coords dest, double direction) {
            return new JObject
                {
                    { "Source", new JObject
                        {
                            { "lat", source.lat }, { "lon", source.lon }
                        } },
                { "Destination", new JObject
                        {
                            { "lat", dest.lat }, { "lon", dest.lon }
                        } },
                { "Direction", direction }
                };

        }

        public static double calcDirection(Coords source, Coords dest) { 
            // From https://www.movable-type.co.uk/scripts/latlong.html
            var y = Math.Sin(dest.lon - source.lon) * Math.Cos(dest.lat);
            var x = Math.Cos(source.lat) * Math.Sin(dest.lat) - Math.Sin(source.lat) * Math.Cos(source.lat) * Math.Cos(dest.lon - source.lon);
            var brng = Math.Atan2(x, y);

            //Convert to degrees and normalise
            return 180 + (brng * (180 / Math.PI));
        }

        private static IActionResult parseParameters(HttpRequest req,out Coords source,out Coords dest) {
            source = new Coords();
            dest = new Coords();

            if (!parseParameters(req, "lon1", out source.lon)) {
                return new BadRequestObjectResult("Invalid lon1 parameter");
            }

            if (!parseParameters(req, "lat1", out source.lat)) {
                return new BadRequestObjectResult("Invalid lat1 parameter");
            }

            if (!parseParameters(req, "lon2", out dest.lon)) {
                return new BadRequestObjectResult("Invalid lon2 parameter");
            }

            if (!parseParameters(req, "lat2", out dest.lon)) {
                return new BadRequestObjectResult("Invalid lat2 parameter");
            }

            return null;
        }

        private static bool parseParameters(HttpRequest req, string param, out double value)
        {
            value = 0;
            string paramValue = req.Query[param];
            if (paramValue == null) { return false; }
            double dvalue;
            if (!double.TryParse(paramValue, out dvalue)) { return false; }
            value = dvalue;
            return true;
        }
    }

    public class Coords {
        public Coords() { }

        public Coords(double la,double lo) {
            lat = la;
            lon = lo;
        }

        public double lat;
        public double lon;
    }
}
