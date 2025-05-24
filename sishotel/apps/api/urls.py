from django.urls import path

from . import views

urlpatterns = [
    path('', views.index),
    path('room', views.room),
    path('getRoomById', views.getRoomById),
    path('reserve', views.reserve),
    path('checkReservedRoom', views.checkReservedRoom),
    path('guest', views.guest),
    path('getGuestById', views.getGuestById),
]