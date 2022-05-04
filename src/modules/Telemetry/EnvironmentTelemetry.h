#pragma once
#include "../mesh/generated/telemetry.pb.h"
#include "ProtobufModule.h"
#include "NodeDB.h"
#include <OLEDDisplay.h>
#include <OLEDDisplayUi.h>

class EnvironmentTelemetryModule : private concurrency::OSThread, public ProtobufModule<Telemetry>
{
  public:
    EnvironmentTelemetryModule()
        : concurrency::OSThread("EnvironmentTelemetryModule"),
          ProtobufModule("EnvironmentTelemetry", PortNum_TELEMETRY_APP, &Telemetry_msg)
    {
        lastMeasurementPacket = nullptr;
    }
    virtual bool wantUIFrame() override;
    virtual void drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y) override;

  protected:
    /** Called to handle a particular incoming message
    @return true if you've guaranteed you've handled this message and no other handlers should be considered for it
    */
    virtual bool handleReceivedProtobuf(const MeshPacket &mp, Telemetry *p) override;
    virtual int32_t runOnce() override;
    /**
     * Send our Telemetry into the mesh
     */
    bool sendOurTelemetry(NodeNum dest = NODENUM_BROADCAST, bool wantReplies = false);

  private:
    Config_ModuleConfig_TelemetryConfig moduleConfig = config.payloadVariant.module_config.payloadVariant.telemetry_config;
    float CelsiusToFahrenheit(float c);
    bool firstTime = 1;
    const MeshPacket *lastMeasurementPacket;
    uint32_t sensor_read_error_count = 0;
};
