using Microsoft.EntityFrameworkCore;
using WeatherBackend.Models;

namespace WeatherBackend.Data
{
    public class WeatherDbContext : DbContext
    {
        public WeatherDbContext(DbContextOptions<WeatherDbContext> options) : base(options) { }

        public DbSet<WeatherData> WeatherEntries { get; set; }
    }
}
