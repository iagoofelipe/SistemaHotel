from rest_framework.decorators import api_view
from rest_framework.response import Response
from rest_framework import status
from datetime import datetime, timedelta

from django.http import HttpRequest

from .models import Room, Guest, Reserve
from .serializers import RoomSerializer, GuestSerializer, ReserveSerializer
from . import tools

CURRENT_DATE = datetime.now()
CURRENT_DATE_PLUS_7 = CURRENT_DATE + timedelta(days=7)
STR_CURRENT_DATE = str(CURRENT_DATE)
STR_CURRENT_DATE_PLUS_7 = str(CURRENT_DATE_PLUS_7)

@api_view(['GET', 'POST', 'PUT', 'DELETE'])
def room(request:HttpRequest):

    match request.method:
        case 'GET':
            checkIn = request.GET.get('checkIn', STR_CURRENT_DATE)
            checkOut = request.GET.get('checkOut', STR_CURRENT_DATE_PLUS_7)

            if checkIn > checkOut:
                return tools.errorResponse('a data de checkIn não pode ser posterior a data de checkOut')

            params = {
                'valMin': 'nightVal__gte',
                'valMax': 'nightVal__lte',
                'value': 'value',
                'numCoupleBed': 'numCoupleBed__gte',
                'numSingleBed': 'numSingleBed__gte',
                'bathtub': 'bathtub',
                'balcony':  'balcony',
            }

            rooms = tools.getFiltered(params, request.GET, Room)
            if not rooms.exists():
                return tools.errorResponse('dados não encontrados para os critérios fornecidos', status.HTTP_404_NOT_FOUND)
            
            rooms_copy = set(rooms)

            for room in rooms:
                if tools.checkReservedRoom(room, checkIn, checkOut):
                    rooms_copy.remove(room)

            return Response(RoomSerializer(rooms_copy, many=True).data)

        case 'POST':
            new_room = request.data
            if 'id' in new_room or 'name' not in new_room:
                return tools.errorResponse()

            serializer = RoomSerializer(data=new_room)
            found = Room.objects.filter(name=new_room.get('name'), floor=new_room.get('floor')).exists()

            if found:
                return tools.errorResponse('já existe um quarto com esse nome e andar', status.HTTP_406_NOT_ACCEPTABLE)

            if serializer.is_valid():
                serializer.save()
                return Response(serializer.data)
        
        case 'DELETE':
            try:
                Room.objects.get(pk=request.data['id']).delete()
                return Response()
            
            except:
                return tools.errorResponse('dados não encontrados', status.HTTP_404_NOT_FOUND)
            
        case 'PUT':
            if 'id' not in request.data:
                return tools.errorResponse()

            try:
                updated_room = Room.objects.get(pk=request.data['id'])
            except:
                return tools.errorResponse('dados não encontrados', status.HTTP_404_NOT_FOUND)
            
            serializer = RoomSerializer(updated_room, data=request.data)

            if serializer.is_valid():
                serializer.save()
                return Response(serializer.data)
            
            return tools.errorResponse()


@api_view(['GET'])
def getRoomById(request:HttpRequest):
    if 'id' not in request.GET:
        return tools.errorResponse()
    
    try:
        room = Room.objects.get(pk=request.GET['id'])
    
    except:
        return tools.errorResponse('quarto não encontrado para o id especificado', status.HTTP_404_NOT_FOUND)

    return Response(RoomSerializer(room).data)


@api_view(['GET', 'POST'])
def reserve(request:HttpRequest):
    match request.method:
        case 'GET':
            params = {
               'guestId': 'guest',
                'roomId': 'room',
                'minCheckIn': 'checkIn__gte',
                'maxCheckIn': 'checkIn__lte',
                'minCheckOut': 'checkOut__gte',
                'maxCheckOut': 'checkOut__lte',
            }

            data = tools.getFiltered(params, request.GET, Reserve)

            if not data.exists():
                return tools.errorResponse('dados não encontrados', status.HTTP_404_NOT_FOUND)

            return Response(ReserveSerializer(data, many=True).data)
        
        case 'POST':
            if 'id' in request.data:
                return tools.errorResponse('id não deve ser fornecido')
            
            serializer = ReserveSerializer(data=request.data)

            if not serializer.is_valid():
                return tools.errorResponse('erro serializer')
            
            try:
                roomId = request.data['room']
                checkIn = request.data['checkIn']
                checkOut = request.data['checkOut']

            except:
                return tools.errorResponse('missing fields')
            
            # caso a data de checkIn seja posterior a data de checkOut
            if checkIn > checkOut:
                return tools.errorResponse('a data de checkIn não pode ser posterior a data de checkOut')
            
            # caso já haja uma reserva para o período nesse quarto
            if tools.checkReservedRoom(roomId, checkIn, checkOut):
                return tools.errorResponse('quarto já reservado para o período desejado', status.HTTP_406_NOT_ACCEPTABLE)

            serializer.save()
            return Response(serializer.data)
            

@api_view(['GET'])
def checkReservedRoom(request:HttpRequest):
    try:
        roomId = request.GET['room']
        checkIn = request.GET['checkIn']
        checkOut = request.GET['checkOut']

    except:
        return tools.errorResponse()

    return Response({ "roomReserved": tools.checkReservedRoom(roomId, checkIn, checkOut) })


@api_view(['GET', 'POST', 'PUT', 'DELETE'])
def guest(request:HttpRequest):
    match request.method:
        case 'GET':
            return Response(GuestSerializer(Guest.objects.all(), many=True).data)

        case 'POST':
            new_guest = request.data
            if 'id' in new_guest or 'name' not in new_guest:
                return tools.errorResponse()

            serializer = GuestSerializer(data=new_guest)
            found = Guest.objects.filter(name=new_guest['name']).exists()

            if found:
                return tools.errorResponse('já existe um cliente cadastrado com esse nome', status.HTTP_406_NOT_ACCEPTABLE)

            if serializer.is_valid():
                serializer.save()
                return Response(serializer.data)
            
            else:
                return tools.errorResponse('erro serializer')
        
        case 'DELETE':
            if 'id' not in request.data:
                return tools.errorResponse()
            
            try:
                Guest.objects.get(pk=request.data['id']).delete()
                return Response()
            
            except:
                return tools.errorResponse('dados não encontrados', status.HTTP_404_NOT_FOUND)
            
        case 'PUT':
            if 'id' not in request.data:
                return tools.errorResponse()

            try:
                updated_guest = Guest.objects.get(pk=request.data['id'])
            except:
                return tools.errorResponse('dados não encontrados', status.HTTP_404_NOT_FOUND)
            
            serializer = GuestSerializer(updated_guest, data=request.data)

            if serializer.is_valid():
                serializer.save()
                return Response(serializer.data)
            
            return tools.errorResponse()