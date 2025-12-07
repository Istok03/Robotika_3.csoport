using WeatherBackend.Models;
using Microsoft.AspNetCore.Mvc;

namespace WeatherBackend.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class WeatherController : ControllerBase
    {
        [HttpPost]
        public IActionResult Post([FromBody] WeatherData data)
        {
            Console.WriteLine($"Temp: {data.Tempature}, Humidity: {data.Humidity}, Pressure: {data.Pressure}");
            return Ok(new { status = "recived"});
        }
       


    }
}
