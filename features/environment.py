def before_scenario(context, scenario):
    context.test_data_dir = "features/test_data"
    context.app_path = "count_pairs.exe"