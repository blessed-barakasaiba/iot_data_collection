from django.shortcuts import render
from rest_framework.decorators import APIView
from . models import *
from . serializers import *
from rest_framework.response import Response
from rest_framework import status

# Create your views here.
class IotdatView(APIView):
    
    def get(self, request):
        try:
            iotdata = Iotdata.objects.all().order_by('-date')
            count = iotdata.count()
            serializer = IotdataSerializer(iotdata, many=True)
            return Response({"count":count, "data":serializer.data})
        except Iotdata.DoesNotExist:
            return Response({"error":"No data found"}, status=status.HTTP_404_NOT_FOUND)
        
    def post(self, request):
        data = request.data
        print(data)  
        deviceid = data['deviceid']
        if deviceid != '1234':
            return Response({"message":"Unauthorized device"}, status=status.HTTP_401_UNAUTHORIZED)
        else:
            serializer = IotdataSerializer(data=data)
            if serializer.is_valid():
                serializer.save()
                return Response({"success":"Submitted successfully"}, status=status.HTTP_201_CREATED)
            return Response({"error":"Failed Try again"})  


      