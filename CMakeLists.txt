cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Definição da versão do SDK e ferramentas
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)

# Inclusão do arquivo do Raspberry Pi Pico VS Code Extension, se existir
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Importa o SDK do Raspberry Pi Pico
include(pico_sdk_import.cmake)

project(Projeto_Analise C CXX ASM)

# Inicializa o SDK do Pico
pico_sdk_init()

# Adiciona o executável e os arquivos necessários
add_executable(Projeto_Analise 
    Projeto_Analise.c 
    inc/ssd1306_i2c.c
)

# Define o nome e a versão do programa
pico_set_program_name(Projeto_Analise "Projeto_Analise")
pico_set_program_version(Projeto_Analise "0.1")

# Geração dos cabeçalhos PIO (garantindo que os arquivos existam)
pico_generate_pio_header(Projeto_Analise ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

# Habilita a saída via UART/USB (pode ser ativado conforme necessário)
pico_enable_stdio_usb(Projeto_Analise 1)
pico_enable_stdio_uart(Projeto_Analise 1)

# Adiciona bibliotecas essenciais
target_link_libraries(Projeto_Analise
    pico_stdlib
    hardware_spi
    hardware_i2c
    hardware_dma
    hardware_pio
    hardware_interp
    hardware_timer
    hardware_watchdog
    hardware_clocks
    hardware_pwm
    hardware_gpio  # Adicionado para uso de LEDs
)

# Inclui diretórios necessários
target_include_directories(Projeto_Analise PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

# Adiciona saídas extras
pico_add_extra_outputs(Projeto_Analise)
