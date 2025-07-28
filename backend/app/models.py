from django.db import models

# Create your models here.
class Iotdata(models.Model):
    temperature = models.DecimalField(decimal_places=2, max_digits=5)
    humidity = models.DecimalField(decimal_places=2, max_digits=5)
    brightnessPercent = models.DecimalField(decimal_places=2, max_digits=5, null=True, blank=True)
    date = models.DateTimeField(auto_now_add=True)
    detected = models.CharField(max_length=50, blank=True, null=True)
    def __str__(self):
        return str(self.temperature)