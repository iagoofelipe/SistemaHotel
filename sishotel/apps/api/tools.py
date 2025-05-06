from .models import Reserve
from django.db.models import Model
from rest_framework.response import Response
from rest_framework import status

def checkReservedRoom(room, checkin, checkout):
    return (
        Reserve.objects.filter(room=room, checkIn__lte=checkin, checkOut__gte=checkin).exists()
        or
        Reserve.objects.filter(room=room, checkIn__lte=checkout, checkOut__gte=checkout).exists()
        or
        Reserve.objects.filter(room=room, checkIn__gt=checkin, checkOut__lt=checkout).exists()
    )

def errorResponse(msg='parâmetros inválidos', status=status.HTTP_400_BAD_REQUEST):
    return Response({'error': msg}, status=status)

def getFiltered(keys:dict[str, str], params:dict, model:Model):
    """
    - keys: { param: filterKey }
    """

    params_to_filter = {}

    for param, key in keys.items():
        val = params.get(param)

        if val is None:
            continue

        params_to_filter[key] = val

    return model.objects.filter(**params_to_filter)