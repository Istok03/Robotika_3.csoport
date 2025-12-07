using Microsoft.EntityFrameworkCore.Metadata.Internal;
using System.ComponentModel.DataAnnotations;

namespace WeatherBackend.Models
{
    public class WeatherData
    {

        [Key]
        public int Id { get; set; }

        [Range(-50,60)]
        [Required]
        public float Temperature { get; set; }
        
        [Range(0,100)]
        public float Humidity { get; set; }
       
        [Range(900,1100)]
        public float Pressure { get; set; }

      

    }
}
