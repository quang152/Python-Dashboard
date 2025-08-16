# 🏠 Smart Home System v4.0.0

[![Version](https://img.shields.io/badge/Version-4.0.0-brightgreen.svg)](https://github.com/your-repo/smart-home-system)
[![Python](https://img.shields.io/badge/Python-3.7+-blue.svg)](https://python.org)
[![Database](https://img.shields.io/badge/Database-Supabase-green.svg)](https://supabase.com)
[![Hardware](https://img.shields.io/badge/Hardware-ESP32_Ready-orange.svg)](https://espressif.com)

> **🎉 Professional Smart Home Platform**  
> ✨ ESP32 Hardware Support • 🔄 Auto Device Discovery • 📊 Dual Database Schema • 🌐 MQTT Integration • 📱 Real-time Dashboard

## 🚀 Quick Start

```bash
# 1. Clone & Setup
git clone <repo> && cd Script-Python
pip install -r requirements.txt

# 2. Configure Environment
cp config/env_template.txt .env
# Edit .env with your Supabase credentials

# 3. Setup Database (Choose One)
python database/setup_simple_schema.py      # Simple schema (recommended)
# OR
python database/setup_enhanced_schema.py    # Enhanced schema (advanced)

# 4. Start System
python src/start_dashboard.py
# → Dashboard: http://localhost:5000
```

## ✨ What's New in v4.0

### 🔧 ESP32 Hardware Integration
- **Real Hardware Support**: Connect ESP32 boards with sensors
- **Plug & Play**: Automatic device discovery via MQTT
- **Multiple Sensors**: DHT22, LDR, PIR support out-of-the-box
- **Wireless Communication**: WiFi → HiveMQ Cloud → Database

### 📊 Dual Database Architecture
- **Simple Schema**: Clean MAC-based IDs, perfect for getting started
- **Enhanced Schema**: Advanced validation, metadata-driven, production-ready
- **Migration Tools**: Seamless upgrade path between schemas

### 🔍 Auto Device Discovery
- **Zero Configuration**: New devices auto-register themselves
- **Smart Validation**: Prevents duplicate and invalid devices
- **User Confirmation**: Accept/reject new devices via dashboard
- **Blacklist Management**: Block unwanted devices

## 📁 Project Architecture

```
Script-Python/
├── 📁 src/                     # Core Application
│   ├── 🚀 start_dashboard.py   # Main launcher
│   ├── 📊 smart_home_dashboard.py  # Web dashboard
│   ├── 📱 smart_home_devices.py    # Device simulator
│   ├── ☁️ cloud_controller.py      # Remote control
│   └── 🌉 mqtt_to_database_bridge* # MQTT-DB bridges
│
├── 📁 config/                  # Configuration
│   ├── ⚙️ config.py           # Main config
│   ├── 📝 smart_home_config.json   # Device definitions
│   ├── 🔧 device_definitions.json  # Enhanced device specs
│   └── 📋 env_template.txt     # Environment template
│
├── 📁 database/                # Database Management
│   ├── 🗄️ supabase_manager_*.py    # DB managers (simple/enhanced)
│   ├── 🛠️ setup_*_schema.py        # Schema setup tools
│   ├── 📄 supabase_schema_*.sql     # SQL schemas
│   ├── 🔄 migrate_to_enhanced.py    # Migration tool
│   └── 🧪 test_*_schema.py         # Testing utilities
│
├── 📁 web/                     # Frontend Assets
│   ├── 📁 templates/           # HTML templates
│   └── 📁 static/             # CSS, JS, images
│
├── 📁 docs/                    # Documentation
│   ├── 📖 ESP32_SETUP_GUIDE.md     # Hardware setup
│   ├── 🔧 ADDING_NEW_DEVICES.md    # Device development
│   ├── 📊 ENHANCED_DATABASE_DESIGN.md  # Advanced DB
│   ├── 🧪 TESTING_GUIDE.md         # Testing procedures
│   └── 🆘 TROUBLESHOOTING.md       # Debug guide
│
├── 📁 utils/                   # Utilities
│   └── 🔍 debug_smart_home.py  # System diagnostics
│
├── 📦 requirements.txt         # Dependencies
├── 🔧 esp32_sensor_client.ino # ESP32 firmware
├── 📋 full_migration_guide.py # Complete migration tool
└── 🚀 safe_start.py           # Safe system launcher
```

## 🎯 Key Features

### 📊 Smart Dashboard
- **Real-time Monitoring**: Live charts with WebSocket updates
- **Device Control**: Interactive controls for all device types  
- **Room Management**: Filter and organize by rooms
- **Mobile Responsive**: Works perfectly on all screen sizes
- **Vietnamese Interface**: Localized UI for Vietnamese users

### 🏠 Device Management  
- **20+ Device Types**: Lights, sensors, switches, cameras, doors, alarms
- **Auto Discovery**: Plug & play device registration
- **Simulated & Real**: Support both virtual and physical devices
- **ESP32 Integration**: Real hardware sensor data
- **MQTT Communication**: Local and cloud broker support

### 🗄️ Database Options

#### Simple Schema (Recommended for Beginners)
```sql
devices:     MAC → Vietnamese Name
rooms:       ID → Vietnamese Name  
sensor_data: Historical readings
```

#### Enhanced Schema (Production Ready)
```sql
device_type_definitions:  Metadata-driven device types
sensor_type_definitions:  Validated sensor specifications
device_capabilities:      Device-sensor compatibility matrix
sensor_data:             Type-safe data with validation
```

## 🔧 Hardware Setup (ESP32)

### 📋 Requirements
- ESP32 development board
- DHT22 (temperature/humidity sensor)
- LDR (light sensor) 
- PIR (motion sensor)
- Breadboard and jumper wires

### ⚡ Quick Hardware Start
```bash
# 1. Wire your sensors (see docs/ESP32_SETUP_GUIDE.md)
# 2. Flash the provided Arduino sketch
# 3. Configure WiFi credentials in sketch
# 4. Start the bridge
python src/mqtt_to_database_bridge.py
# 5. Power on ESP32 → Auto-discovery kicks in!
```

📖 **Detailed Guide**: [`docs/ESP32_SETUP_GUIDE.md`](docs/ESP32_SETUP_GUIDE.md)

## 🎮 Usage

### Starting the System

```bash
# Option 1: All-in-one launcher (Recommended)
python src/start_dashboard.py

# Option 2: Individual components
python src/smart_home_devices.py     # Device simulator
python src/cloud_controller.py       # MQTT controller  
python src/smart_home_dashboard.py   # Web dashboard
```

### System Management

```bash
# System Diagnostics
python utils/debug_smart_home.py

# Database Testing
python database/test_simple_schema.py

# ESP32 Connection Test
python test_esp32_connection.py

# Device Discovery Test
python test_device_discovery.py
```

## 🔍 Device Discovery System

### 📡 How It Works
```
1. ESP32/Device Powers On
   ↓
2. Publishes Discovery Message via MQTT
   ↓
3. Dashboard Receives & Validates
   ↓
4. Shows Confirmation Dialog
   ↓
5. User Accepts → Added to Database
   User Rejects → Added to Blacklist
```

### 🧪 Testing Discovery
```bash
# Method 1: Use test script
python test_device_discovery.py

# Method 2: Restart device simulator  
python src/smart_home_devices.py

# Method 3: Manual MQTT publish
mosquitto_pub -h localhost -t "devices/light/AA:BB:CC:DD:EE:FF/discovery" \
  -m '{"mac_address":"AA:BB:CC:DD:EE:FF","name":"Test Light","device_type":"light"}'
```

## 🚀 Migration & Scaling

### Database Migration
```bash
# Simple → Enhanced Schema
python full_migration_guide.py  # Guided migration
# OR
python database/migrate_to_enhanced.py  # Direct migration
```

### Adding New Device Types
📖 **Step-by-step Guide**: [`docs/ADDING_NEW_DEVICES.md`](docs/ADDING_NEW_DEVICES.md)

**Quick Overview**:
1. Define in `config/smart_home_config.json`
2. Implement logic in `src/smart_home_devices.py`
3. Add controls in `src/cloud_controller.py`
4. Test integration

## 🌐 Architecture Overview

```
┌─────────────┐    ┌──────────────┐    ┌─────────────┐    ┌──────────────┐
│   Browser   │◄──►│ Flask/Socket │◄──►│ MQTT Broker │◄──►│   Devices    │
│   (Web UI)  │    │  (Dashboard) │    │ (Local/Cloud)│    │ (ESP32/Sim)  │
└─────────────┘    └──────────────┘    └─────────────┘    └──────────────┘
        │                   │                   │                   │
    Charts &            Real-time            Command              Sensor
    Controls            WebSocket            Routing              Data
        │                   │                   │                   │
        └───────────────────┼───────────────────┴───────────────────┘
                            ▼
                    ┌──────────────┐
                    │   Supabase   │
                    │   Database   │
                    └──────────────┘
```

## 📋 Supported Devices

| Device Type | Commands | Sensors | Hardware Ready |
|-------------|----------|---------|----------------|
| 💡 **Smart Light** | on, off, dim, color | brightness, power | ✅ ESP32 + LED |
| 🌡️ **Temperature Sensor** | calibrate | temp, humidity | ✅ ESP32 + DHT22 |
| 🔌 **Smart Switch** | on, off, toggle | power, current | ✅ ESP32 + Relay |
| 📹 **Camera** | record, capture | motion, light | 🔄 In Progress |
| 🚪 **Smart Door** | lock, unlock | status, access | 🔄 Planned |
| 🚨 **Alarm System** | arm, disarm | sensors, alerts | 🔄 Planned |

## 🆘 Troubleshooting

```bash
# Step 1: Run comprehensive diagnostics
python utils/debug_smart_home.py

# Step 2: Check specific issues
python database/test_simple_schema.py      # Database
python test_esp32_connection.py            # Hardware
python test_device_discovery.py            # Discovery

# Step 3: Consult documentation
# See docs/TROUBLESHOOTING.md for detailed solutions
```

### Common Issues & Solutions

| Issue | Solution |
|-------|----------|
| 🔢 Device count shows 0 | Run `python database/setup_simple_schema.py` |
| 📊 Empty room chart | Check `python database/test_simple_schema.py` |
| 🔍 Discovery not working | Verify MQTT broker: `netstat -an \| grep 1883` |
| 📱 ESP32 not connecting | Check WiFi credentials and HiveMQ settings |

## 📚 Documentation

| Guide | Description |
|-------|-------------|
| 📖 [`docs/ESP32_SETUP_GUIDE.md`](docs/ESP32_SETUP_GUIDE.md) | Complete ESP32 hardware setup |
| 🔧 [`docs/ADDING_NEW_DEVICES.md`](docs/ADDING_NEW_DEVICES.md) | Adding custom device types |
| 📊 [`docs/ENHANCED_DATABASE_DESIGN.md`](docs/ENHANCED_DATABASE_DESIGN.md) | Advanced database architecture |
| 🧪 [`docs/TESTING_GUIDE.md`](docs/TESTING_GUIDE.md) | Testing procedures and tools |
| 🆘 [`docs/TROUBLESHOOTING.md`](docs/TROUBLESHOOTING.md) | Debug guide and solutions |

## 🎯 Next Steps

### For Beginners
1. **Start Simple**: Use simple schema and device simulator
2. **Learn the Dashboard**: Explore room management and device controls
3. **Add Your First ESP32**: Follow the ESP32 setup guide

### For Advanced Users  
1. **Migrate to Enhanced Schema**: Better validation and scalability
2. **Build Custom Devices**: Add new device types and sensors
3. **Deploy Production**: Set up cloud MQTT and persistent database

### For Developers
1. **Extend the Platform**: Add new features and integrations
2. **Contribute**: Improve documentation and add device types
3. **Scale Up**: Multi-home support and advanced automation

---

## 📊 Version History

- **v4.0.0**: ESP32 hardware integration, dual database architecture, auto-discovery
- **v3.0.1**: Device discovery system, organized folder structure
- **v3.0.0**: Simple schema migration, UI fixes, Vietnamese localization
- **v2.x**: MQTT integration, device simulation
- **v1.x**: Basic dashboard and device management

---

**🎉 Ready to build your smart home! Start with: `python src/start_dashboard.py`**

*Need help? Check our comprehensive documentation in the `docs/` folder or run `python utils/debug_smart_home.py` for system diagnostics.* 