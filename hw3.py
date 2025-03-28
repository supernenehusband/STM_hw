from bluepy.btle import Peripheral, UUID, Scanner, DefaultDelegate
import time

# 接收 Notification 的 callback
class NotificationDelegate(DefaultDelegate):
    def __init__(self):
        super().__init__()

    def handleNotification(self, cHandle, data):
        print(f"[Notify] 🔥 Received from {cHandle}: {data}")

# 掃描 BLE 裝置
scanner = Scanner().withDelegate(DefaultDelegate())
print("🔍 Scanning for 2 seconds...")
devices = scanner.scan(2.0)

# 列出裝置
addr = []
for i, dev in enumerate(devices):
    print(f"{i}: Device {dev.addr} ({dev.addrType}), RSSI={dev.rssi} dB")
    addr.append(dev.addr)
    for (adtype, desc, value) in dev.getScanData():
        print(f"   {desc}: {value}")

# 選擇裝置
number = int(input("Enter your device number: "))
print("📡 Connecting to", addr[number])
dev = Peripheral(addr[number], 'random')
dev.setDelegate(NotificationDelegate())

# 顯示服務
print("📋 Services:")
for svc in dev.services:
    print(f"  - {svc}")

try:
    # 尋找自定義 Service & Characteristic
    service_uuid = UUID(0xfff0)
    char_uuid = UUID(0xfff1)

    testService = dev.getServiceByUUID(service_uuid)
    ch = dev.getCharacteristics(uuid=char_uuid)[0]

    print(f"\n✅ Characteristic {char_uuid}:")
    print("  Properties:", ch.propertiesToString())

    if ch.supportsRead():
        initial_value = ch.read()
        print(f"📥 Initial value from {char_uuid}: {initial_value}")

    # 確認是否支援 Notify
    if "NOTIFY" not in ch.propertiesToString():
        print("❌ This characteristic does NOT support notifications.")
    else:
        print("🔔 Supports notifications!")

        # 取得 CCCD 描述元
        descriptors = ch.getDescriptors()
        cccd_found = False
        for desc in descriptors:
            print(f"  Found descriptor: {desc.uuid}")
            if desc.uuid == UUID(0x2902):
                print(f"✳️ Writing to CCCD handle {desc.handle}")
                dev.writeCharacteristic(desc.handle, b'\x02\x00', withResponse=False)
                print("✅ CCCD set to 0x0002 (Notifications enabled)")
                cccd_found = True
                break

        if not cccd_found:
            print("❌ CCCD descriptor (0x2902) not found. Cannot enable notifications.")

        # 等待通知
        # 初始讀取特徵值
        last_value = None
        if ch.supportsRead():
            last_value = ch.read()
            print(f"📥 Initial value: {last_value}")

        print("\n⏳ Waiting for notifications or value changes (Press Ctrl+C to stop)...")
        while True:
            # 等 notification（如有）
            if dev.waitForNotifications(1.0):
                continue

            # 輪詢特徵值
            if ch.supportsRead():
                current_value = ch.read()
                if current_value != last_value:
                    print(f"🔁 Value changed: {current_value}")
                    last_value = current_value


except KeyboardInterrupt:
    print("\n🛑 Interrupted by user.")
finally:
    dev.disconnect()
    print("🔌 Disconnected.")
