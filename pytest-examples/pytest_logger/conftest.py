def pytest_logger_config(logger_config):
    loggers = ['foo', 'bar']
    logger_config.add_loggers(loggers, file_level='debug', stdout_level='info')
    logger_config.set_log_option_default('foo.info')
