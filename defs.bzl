"""Rules for building FFMpeg
"""

TxTypeProviderInfo = provider("FFmpeg tx type choice.", fields = ["tx_type"])

types = ["float", "double", "int32"]

def _impl(ctx):
    raw_type = ctx.build_setting_value
    if raw_type not in types:
        fail(str(ctx.label) + " build setting allowed to take values {" +
             ", ".join(types) + "} but was set to unallowed value " +
             raw_type)
    return TxTypeProvider(type = raw_temperature)

tx_type = rule(
    implementation = _impl,
    build_setting = config.string(flag = True),
)
