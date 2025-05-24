from rest_framework import serializers

from .models import Room, Guest, Reserve

class RoomSerializer(serializers.ModelSerializer):
    class Meta:
        model = Room
        fields = '__all__'

class GuestSerializer(serializers.ModelSerializer):
    class Meta:
        model = Guest
        fields = '__all__'

class ReserveSerializer(serializers.ModelSerializer):
    roomId = serializers.CharField(source="room.id", read_only=True)
    roomName = serializers.CharField(source="room.name", read_only=True)
    roomFloor = serializers.IntegerField(source="room.floor", read_only=True)
    guestId = serializers.CharField(source="guest.id", read_only=True)
    guestName = serializers.CharField(source="guest.name", read_only=True)

    class Meta:
        model = Reserve
        # fields = ['id', 'room', 'guestName', 'checkin', 'chekout']
        # fields = '__all__'
        fields = ['id', 'roomId', 'roomName', 'roomFloor', 'guestId', 'guestName', 'checkIn', 'checkOut']