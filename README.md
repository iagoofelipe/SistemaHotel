# SistemaHotel
Aplicação para o gerencimanto de quartos de um hotel, utilizando Python.

# API
Endpoints disponíveis na API.

## /api/room

### GET
Retorna os quartos disponíveis para o período informado. Aceita filtros de pesquisa como parâmetros.

| Parâmetro | Tipo | Valor Padrão | Descrição |
| --- | --- | --- | --- |
| **Obrigatório** |
| checkIn | datetime | \<`CURRENT_DATETIME`> | data/hora de checkIn desejado |
| checkOut | datetime | \<`CURRENT_DATETIME` + 7 dias> | data/hora de checkOut desejado |
| **Opcional** |
| valMin | double | | valor mínimo por noite |
| valMax | double | | valor máximo por noite |
| value | double | | valor por noite |
| numCoupleBed | int | | número mínimo de camas de casal |
| numSingleBed | int | | número mínimo de camas de solteiro |
| bathtub | boolean | | se deve incluir banheira |
| balcony | boolean | | se deve incluir varanda |

#### Respostas

| Código | Retorno | Descrição |
| --- | --- | --- |
| 200 | [Room[]](#room) | Retorna um array contendo os quartos compatíveis com os parâmetros especificados |
| 400 | [ErrorResponse](#errorresponse) | má requisição |
| 404 | [ErrorResponse](#errorresponse) | não encontrado |

### POST
Cria um novo quarto com os parâmetros especificados.

| Parâmetro | Tipo | Descrição |
| --- | --- | --- |
| [RoomCreate](#roomcreate) | object | parâmetros do quarto a ser criado |


#### Respostas

| Código | Retorno | Descrição |
| --- | --- | --- |
| 200 | [Room](#room) | dados do quarto criado criado |
| 406 | [ErrorResponse](#errorresponse) | não aceitável |
| 400 | [ErrorResponse](#errorresponse) | Bad Request |

### DELETE
Deleta um quarto específico da base de dados.

| Parâmetro | Tipo | Descrição |
| --- | --- | --- |
| id | string | id do quarto |

#### Respostas

| Código | Retorno | Descrição |
| --- | --- | --- |
| 200 | - | - |
| 404 | [ErrorResponse](#errorresponse) | quarto não encontrado |

### PUT
Atualiza os dados de um quarto existente. Aceita um objeto [Room](#room) como entrada e utiliza o parâmetro `id` como chave de pesquisa. Caso o mesmo não seja encontrado, um erro `404` é retornado.

> Obs: apenas os dados fornecidos serão atualizados, com exceção de `id`

| Parâmetro | Tipo | Descrição |
| --- | --- | --- |
| [Room](#room) | object | parâmetros do quarto a ser atualizado |


#### Respostas

| Código | Retorno | Descrição |
| --- | --- | --- |
| 200 | [Room](#room) | dados do quarto atualizado |
| 400 | [ErrorResponse](#errorresponse)  | má requisição |
| 404 | [ErrorResponse](#errorresponse)  | quarto não encontrado |

## /api/getRoomById

### GET
Consulta um quarto por id.

| Parâmetro | Tipo | Descrição |
| --- | --- | --- |
| id | string | id do quarto |

#### Respostas

| Código | Retorno | Descrição |
| --- | --- | --- |
| 200 | [Room](#room) | objeto do quarto |
| 404 | - | não encontrado |
| 400 | - | Bad Request |

## /api/reserve

### GET
Retorna todas as reservas feitas.

| Parâmetro | Tipo | Valor Padrão | Descrição |
| --- | --- | --- | --- |
| guestId | string | | id do hóspede |
| roomId | string | | id do quarto |
| minCheckIn | datetime | | data/hora mínima de check-in |
| maxCheckIn | datetime | | data/hora máxima de check-in |
| minCheckOut | datetime | | data/hora mínima de check-out |
| maxCheckOut | datetime | | data/hora máxima de check-out |

#### Respostas

| Código | Retorno | Descrição |
| --- | --- | --- |
| 200 | [Reserve[]](#reserve) | array de reservas |
| 404 | [ErrorResponse](#errorresponse) | erro de busca |

### POST
Cria uma nova reserva.

| Parâmetro | Tipo |
| --- | --- |
| [ReserveCreate](#reservecreate) | object |

#### Respostas

| Código | Retorno | Descrição |
| --- | --- | --- |
| 200 | [Reserve](#reserve) | dados da reserva |
| 400 | [ErrorResponse](#errorresponse) | erro de validação |
| 406 | [ErrorResponse](#errorresponse) | erro ao reservar o quarto |

## /api/checkReservedRoom

### GET
Retorna se o quarto já está reservado na data informada.

| Parâmetro | Tipo | Descrição |
| --- | --- | --- |
| room | string |
| checkIn | datetime |
| checkOut | datetime |

#### Respostas

| Código | Retorno | Descrição |
| --- | --- | --- |
| 200 | [RoomReserved](#roomreserved) |
| 400 | [ErrorResponse](#errorresponse) |

## /api/guest

### GET
Retorna todos os clientes cadastrados.

#### Respostas
| Código | Retorno | Descrição |
| --- | --- | --- |
| 200 | [Guest[]](#guest) | array de clientes |
| 404 | [ErrorResponse](#errorresponse) | dados não encontrados |

### POST
Cria um novo cliente com os parâmetros especificados.

| Parâmetro | Tipo | Descrição |
| --- | --- | --- |
| [GuestCreate](#guestcreate) | object | parâmetros do cliente a ser criado |


#### Respostas

| Código | Retorno | Descrição |
| --- | --- | --- |
| 200 | [Guest](#guest) | dados do cliente criado criado |
| 406 | [ErrorResponse](#errorresponse) | não aceitável |
| 400 | [ErrorResponse](#errorresponse) | Bad Request |

### DELETE
Deleta um cliente específico da base de dados.

| Parâmetro | Tipo | Descrição |
| --- | --- | --- |
| id | string | id do cliente |

#### Respostas

| Código | Retorno | Descrição |
| --- | --- | --- |
| 200 | - | - |
| 400 | [ErrorResponse](#errorresponse) | parâmetros inválidos |
| 404 | [ErrorResponse](#errorresponse) | cliente não encontrado |

### PUT
Atualiza os dados de um quarto existente. Aceita um objeto [Guest](#guest) como entrada e utiliza o parâmetro `id` como chave de pesquisa. Caso o mesmo não seja encontrado, um erro `404` é retornado.

> Obs: apenas os dados fornecidos serão atualizados, com exceção de `id`

| Parâmetro | Tipo | Descrição |
| --- | --- | --- |
| [Guest](#guest) | object | parâmetros do quarto a ser atualizado |


#### Respostas

| Código | Retorno | Descrição |
| --- | --- | --- |
| 200 | [Guest](#guest) | dados do cliente atualizado |
| 400 | [ErrorResponse](#errorresponse)  | má requisição |
| 404 | [ErrorResponse](#errorresponse)  | cliente não encontrado |

## /api/guestById

### GET
Retorna os dados do cliente especificado.

| Parâmetro | Tipo | Descrição |
| --- | --- | --- |
| id | string | id do cliente |

#### Respostas
| Código | Retorno | Descrição |
| --- | --- | --- |
| 200 | [Guest](#guest) | dados do cliente |
| 404 | [ErrorResponse](#errorresponse) | dados não encontrados |


## Estruturas

### ErrorResponse

| Parâmetro | Tipo |
| --- | --- |
| error | string |

### Room

| Parâmetro | Tipo |
| --- | --- |
| id | string |
| name | string |
| floor | num |
| nightVal | double |
| hourVal | double |
| numCoupleBed | int |
| numSingleBed | int |
| bathtub | boolean |
| balcony | boolean |

### RoomCreate

| Parâmetro | Tipo | Valor Padrão |
| --- | --- | --- |
| name | string |
| floor | num | 0 |
| nightVal | double | 0 |
| hourVal | double | 0 |
| numCoupleBed | int | 0 |
| numSingleBed | int | 0 |
| bathtub | boolean | False |
| balcony | boolean | False |

### RoomReserved

| Parâmetro | Tipo | Valor Padrão |
| --- | --- | --- |
| roomReserved | boolean | False |

### Guest

| Parâmetro | Tipo |
| --- | --- |
| id | string |
| name | string |
| email | string |

### GuestCreate

| Parâmetro | Tipo | Valor Padrão |
| --- | --- | --- |
| name | string |
| email | string |

### Reserve

| Parâmetro | Tipo |
| --- | --- |
| id | string |
| room | string |
| guest | string |
| checkIn | datetime |
| checkOut | datetime |

### ReserveCreate

| Parâmetro | Tipo | Valor Padrão |
| --- | --- | --- |
| room | string |
| guest | string |
| checkIn | datetime |
| checkOut | datetime |

## Enumerações
