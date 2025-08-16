#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Smart Home System Setup - SIMPLE VERSION
Setup database và test connection
"""

import os
from dotenv import load_dotenv


def check_environment():
    """Kiểm tra environment variables"""
    print("🔍 Checking environment variables...")

    # Load .env file
    load_dotenv()

    # Check required variables
    url = os.getenv("SUPABASE_URL")
    key = os.getenv("SUPABASE_KEY")

    if not url or not key:
        print("❌ Missing Supabase credentials!")
        print("   Please check your .env file has:")
        print("   SUPABASE_URL=https://your-project.supabase.co")
        print("   SUPABASE_KEY=your-anon-key-here")
        return False

    print(f"✅ SUPABASE_URL: {url[:30]}...")
    print(f"✅ SUPABASE_KEY: {key[:20]}...")
    return True


def test_database_connection():
    """Test kết nối database"""
    print("\n📡 Testing database connection...")

    try:
        from database.supabase_manager_device_based import SupabaseManagerDeviceBased

        manager = SupabaseManagerDeviceBased()

        # Test connection
        devices = manager.get_all_devices()
        print(f"✅ Database connected! Found {len(devices)} devices")
        return True

    except Exception as e:
        print(f"❌ Database connection failed: {e}")
        return False


def show_next_steps():
    """Hướng dẫn bước tiếp theo"""
    print("\n🚀 Next Steps:")
    print("=" * 40)
    print("1. Setup database schema:")
    print("   • Open Supabase SQL Editor")
    print("   • Copy & paste: database/supabase_schema_latest.sql")
    print("   • Run the SQL script")
    print()
    print("2. Start the dashboard:")
    print("   python src/start_dashboard.py")
    print()
    print("3. Access web interface:")
    print("   http://localhost:5000")


def main():
    """Main setup function"""
    print("🏠 Smart Home System Setup")
    print("=" * 40)

    # Step 1: Check environment
    if not check_environment():
        print("\n💡 Fix .env file first, then run this script again")
        return False

    # Step 2: Test database
    if not test_database_connection():
        print("\n💡 Setup database schema first (see next steps below)")
        show_next_steps()
        return False

    # Success
    print("\n🎉 Smart Home System Ready!")
    print("✅ Environment variables OK")
    print("✅ Database connection OK")
    print("✅ Ready to use!")

    print("\n🚀 Start dashboard with:")
    print("   python src/start_dashboard.py")

    return True


if __name__ == "__main__":
    main()
