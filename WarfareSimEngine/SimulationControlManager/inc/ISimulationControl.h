

enum class SimulationState
{
    STOPPED,
    RUNNING,
    PAUSED
};

class ISimulationControl
{
public:
    virtual ~ISimulationControl() = default;

    virtual bool Start() = 0;
    virtual bool Pause() = 0;
    virtual bool Resume() = 0;
    virtual bool Stop() = 0;
};