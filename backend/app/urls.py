from . views import *
from django.urls import path

urlpatterns = [
    path("data/",IotdatView.as_view(),),
]
