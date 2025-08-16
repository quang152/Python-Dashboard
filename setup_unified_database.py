#!/usr/bin/env python3
"""
Setup script for unified database schema
Tạo database và test kết nối
"""

import sys
import os
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from src.database_manager_unified import SmartHomeDatabaseUnified

def main():
    print("🏠 Smart Home Unified Database Setup")
    print("=" * 50)
    
    try:
        # Test database connection
        print("1. 🔌 Testing database connection...")
        db = SmartHomeDatabaseUnified()
        
        if db.is_connected():
            print("   ✅ Database connection successful!")
        else:
            print("   ❌ Database connection failed!")
            return
        
        # Get sample devices
        print("\n2. 📱 Getting devices from database...")
        devices = db.get_all_devices()
        print(f"   ✅ Found {len(devices)} devices")
        
        for device in devices:
            print(f"   📍 {device['device_name']} ({device['device_type']}) - {device['room']}")
        
        # Test dashboard overview
        print("\n3. 📊 Testing dashboard overview...")
        overview = db.get_dashboard_overview()
        print(f"   ✅ Dashboard overview: {len(overview)} devices with status")
        
        # Test room summary
        print("\n4. 🏠 Testing room summary...")
        rooms = db.get_room_summary()
        print(f"   ✅ Room summary: {len(rooms)} room/device combinations")
        
        for room in rooms:
            print(f"   🏠 {room['room']}: {room['device_count']} {room['device_type']} devices ({room['online_count']} online)")
        
        # Test recent activities
        print("\n5. 📈 Testing recent activities...")
        activities = db.get_device_activities()
        print(f"   ✅ Recent activities: {len(activities)} activities")
        
        print("\n🎉 Unified database setup completed successfully!")
        print("✅ All tests passed - Ready to use!")
        
    except Exception as e:
        print(f"❌ Error during setup: {e}")
        return False
    
    return True

if __name__ == "__main__":
    success = main()
    if success:
        print("\n🚀 You can now start the dashboard with: python src/start_dashboard.py")
    else:
        print("\n❌ Setup failed. Please check your database configuration.")