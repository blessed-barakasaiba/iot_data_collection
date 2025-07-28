from rest_framework import serializers
from .models import *

class IotdataSerializer(serializers.ModelSerializer):
    class Meta:
        model = Iotdata
        fields = "__all__"
        