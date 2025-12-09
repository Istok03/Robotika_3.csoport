using WeatherBackend.Models;
using Microsoft.AspNetCore.Mvc;
using WeatherBackend.Data;
using Microsoft.EntityFrameworkCore;

namespace WeatherBackend.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class WeatherController : ControllerBase
    {
        [HttpPost]

        public async Task<IActionResult> Post([FromBody] WeatherData data)
        {
            _context.WeatherEntries.Add(data);
            await  _context.SaveChangesAsync();

            Console.WriteLine($"Temp: {data.Temperature}, Humidity: {data.Humidity}, Pressure: {data.Pressure}");
            return Ok(new { status = "recived"});
        }


        private readonly WeatherDbContext _context;
        public WeatherController(WeatherDbContext context)
        {
            _context = context;
        }

        [HttpGet]

       

        public async Task<IActionResult> GetAll()
        {
            var entries = await _context.WeatherEntries.ToListAsync();
            return Ok(entries);
        }

    }
}
