using WeatherBackend.Models;
using Microsoft.AspNetCore.Mvc;
using WeatherBackend.Data;
using Microsoft.EntityFrameworkCore;
using System.Text;

namespace WeatherBackend.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class WeatherController : ControllerBase
    {
        [HttpPost]

        public async Task<IActionResult> Post([FromBody] WeatherData data)
        {
            data.Timestamp = DateTime.Now;
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

        [HttpGet("export")]
        public async Task<IActionResult> ExportToCsv()
        {
            var entries = await _context.WeatherEntries.ToListAsync();

            var csvBuilder = new StringBuilder();
            csvBuilder.AppendLine("Id;Temperature;Humidity;Pressure;Timestamp");

            foreach(var entry in entries)
            {
                csvBuilder.AppendLine($"{entry.Id};{entry.Temperature};{entry.Humidity};{entry.Pressure};{entry.Timestamp:yyyy-MM-dd HH:mm:ss}");
            }
            var csvBytes = Encoding.UTF8.GetBytes(csvBuilder.ToString());
            return File(csvBytes, "text/csv", "weather_data.csv");

        }

    }
}
