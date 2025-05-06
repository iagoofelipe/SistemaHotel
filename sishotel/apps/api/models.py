from django.db import models
from uuid import uuid4

DEFAULT_DATE = '2025-01-01 00:00:00'

class Room(models.Model):
    id = models.CharField(default=uuid4, primary_key=True)
    name = models.CharField(max_length=150, default='')
    floor = models.IntegerField(default=0)
    nightVal = models.DecimalField(default=0, max_digits=9, decimal_places=2)
    hourVal = models.DecimalField(default=0, max_digits=9, decimal_places=2)
    numCoupleBed = models.IntegerField(default=0)
    numSingleBed = models.IntegerField(default=0)
    bathtub = models.BooleanField(default=False)
    balcony = models.BooleanField(default=False)

class Guest(models.Model):
    id = models.CharField(default=uuid4, primary_key=True)
    name = models.CharField(max_length=150, default='')
    email = models.EmailField(default='')

class Reserve(models.Model):
    id = models.CharField(default=uuid4, primary_key=True)
    room = models.ForeignKey(Room, default=None, on_delete=models.CASCADE)
    guest = models.ForeignKey(Guest, default=None, on_delete=models.CASCADE)
    checkIn = models.DateTimeField(default=DEFAULT_DATE)
    checkOut = models.DateTimeField(default=DEFAULT_DATE)